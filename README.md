# tcc-verified — cryptographic proof of compilation

A proof-of-concept that produces a **succinct cryptographic proof that a binary
is the result of compiling a given C source file** — verifiable in
milliseconds, by anyone, without a compiler, toolchain, or re-execution.

[TinyCC](https://repo.or.cz/tinycc.git) is cross-compiled to riscv32im and runs
*inside* the [RISC Zero](https://github.com/risc0/risc0) zkVM. The guest
compiles the source entirely in memory into a statically-linked x86-64 Linux
ELF and commits to the journal:

```
sha256(source) || git-blob-sha1(source) || sha256(binary)      (84 bytes)
```

The git blob hash (`git hash-object source.c`) links the proof to a git commit
through the ordinary commit → tree → blob hash chain, which is publicly
checkable outside the proof.

## The proof statement

> There exists an execution of the compiler image `IMAGE_ID` that read a source
> file with hash `sha256(source)` (= git blob `B`) and produced output with
> hash `sha256(binary)`.

`IMAGE_ID` pins **everything**: the TCC binary (vendored at commit
`vendor/tinycc/VENDORED_COMMIT`, built with the flags in
`methods/guest/build.rs`), the compiler options (`-static -nostdlib`, baked
into `methods/guest/shim/compile.c`), the newlib syscall shims, and the Rust
guest wrapper. Trusting a receipt means trusting (a) the RISC Zero proof
system, and (b) that this pinned compiler is an honest compiler — the proof
removes trust in the *builder*, not in the compiler (Thompson's
trusting-trust reduction still applies; you can audit the vendored sources).

The compilation runs single-threaded with a pinned empty environment and
`time() = 0`, so it is fully deterministic — but the proof doesn't even
require that: it attests "this output is *a* valid compilation", which is what
provenance needs.

## Usage

```sh
rzup install                      # RISC Zero toolchains (rust, cpp, r0vm)

# prove (pick one mode; groth16 needs a docker-compatible CLI — podman works:
#   systemctl --user start podman.socket
#   export DOCKER_HOST=unix://$XDG_RUNTIME_DIR/podman/podman.sock )
cargo run --release -p host -- examples/hello.c --mode composite
cargo run --release -p host -- examples/hello.c --mode succinct
cargo run --release -p host -- examples/hello.c --mode groth16

./out/hello                       # the freshly proven binary
# => hello world

# verify: no toolchain, no re-execution, milliseconds
cargo run --release -p verifier -- out/receipt.groth16.bin out/hello examples/hello.c
```

Iterate without proving: `cargo run -p host -- examples/hello.c --dev`
(`RISC0_DEV_MODE`, fake receipt).

## Measurements

Compilation cost is linear in source size (~450 guest cycles/byte; TCC
startup ≈ 1.2 M cycles). Execution is ~25 MHz emulated — proving is the
entire cost. Receipt sizes and verification are constant: 223 KB / ~18 ms
(succinct), **665 B / ~5 ms (groth16)**, any program size.

**Proving wall-time** (succinct; local CPU = 8-core AVX-512, GPU = rented
RTX 4090 @ $0.39/hr via `cloud/prove-remote.sh`):

| source | size | segments | CPU | GPU | GPU cost |
|---|---:|---:|---:|---:|---:|
| hello.c | 619 B | 2 | 1258 s | **14.9 s** | <0.2¢ |
| bench_small.c | 5.9 KB | 5 | — | **19.2 s** | ~0.2¢ |
| bench_medium.c | 24.9 KB | 14 | — | **33.1 s** | ~0.4¢ |
| bench_large.c | 103 KB | 50 | ~9 h (est.) | **89.6 s** | ~1¢ |

GPU scaling: ~12 s fixed + ~1.55 s/segment (1 segment = 2²⁰ cycles ≈ 2.3 KB
of source). The groth16 wrap adds a constant ~160-230 s on the local CPU
(`host --wrap`, podman) and can run anywhere.

Local CPU groth16 end-to-end (hello.c): composite 1101 s / succinct 1258 s /
groth16 1557 s — i.e. the GPU is ~85-350× faster and a verified compilation
of 100 KB of C costs about a cent.

Native `tcc` compiles these in ~1 ms; the proving overhead is paid once,
while every verification afterwards is milliseconds, forever.

## Cloud GPU proving (vast.ai)

Proving is separable from compilation: execution runs at ~25 MHz emulated
and segments serialize to ~250 KB each, so the heavy part — segment STARKs —
can run anywhere. Receipts are self-verifying, which makes the cheap spot
GPU market safe: a bad host can fail to deliver, but never forge.

```sh
pip install vastai && vastai set api-key <KEY>   # + ssh key in account, ~$10 balance
cloud/prove-remote.sh examples/bench_large.c     # rent 4090 -> prove -> fetch -> wrap -> verify
```

**The canonical compiler is the CI-baked image** (`cloud/Dockerfile` →
ghcr.io/nikicat/tcc-verified-prover, built by GitHub Actions): guest builds
are only reproducible inside a fixed environment, so the image's guest IS
the pinned compiler (`CANONICAL_IMAGE_ID`, enforced by CI on every bake —
independently reproduced by two builders). Instances boot it prebuilt
(~4 min image pull, zero remote building), prove the **succinct** receipt
on GPU, and the driver wraps to **groth16 locally** through podman
(`host --wrap`, avoids docker-in-docker on container rentals), verifies
against the pin, and destroys the instance. Rebakes reuse the cargo-chef
registry cache: ~15 min instead of ~60 (the CUDA kernel layer only rebuilds
when dependencies change).

Measured end-to-end (cold instance to verified groth16): hello.c ≈ 8 min
wall, of which 17 s is GPU proving and ~3 min the local wrap; marginal cost
per proof ≈ 1¢ at bench_large size (see Measurements).

## Layout

- `examples/` — freestanding programs (raw `syscall`, no libc — the pinned
  statement is single-file `-nostdlib`, so the produced ELFs depend on
  nothing outside the proof): `hello.c`; realistic kernels `fib.c`,
  `primes.c` (sieve to 10⁶), `sort.c` (recursive quicksort + FNV
  self-check) whose proven binaries compute verifiably correct results,
  each ~12 s GPU proving; synthetic `bench_*.c` for size scaling
- `vendor/tinycc` — pinned TCC sources (see `VENDORED_COMMIT`)
- `methods/guest/` — zkVM guest: `build.rs` cross-compiles TCC
  (`ONE_SOURCE`, `TCC_TARGET_X86_64`, `CONFIG_TCC_PREDEFS`), `shim/sys.c` is
  the newlib OS layer (bump-`sbrk`, in-memory output file, no real syscalls),
  `shim/compile.c` drives libtcc, `src/main.rs` hashes + commits the journal
- `host/` — proves a compilation, writes `out/hello` + `out/receipt.*.bin`
- `verifier/` — standalone verification: `receipt.verify(IMAGE_ID)` + hash
  comparison

## Prior art

[CosmicTurtle (arXiv:2602.11887)](https://arxiv.org/abs/2602.11887) proved
chibicc compilations in RISC Zero, source → x86-64 *assembly text*, with
uncompressed composite receipts (MB-scale, slow verification). This PoC
differs in producing a *runnable linked ELF* inside the proof (TCC's
in-memory linker) and compressing to constant-size succinct/Groth16 receipts
that verify in milliseconds.
