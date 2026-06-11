# tcc-verified — cryptographic proof of compilation

A proof-of-concept that produces a **succinct cryptographic proof that a binary
is the result of compiling given C sources** — verifiable in
milliseconds, by anyone, without a compiler, toolchain, or re-execution.
It scales from a single freestanding file to a real multi-TU program
linked against musl libc (`examples/musl-hello`).

[TinyCC](https://repo.or.cz/tinycc.git) is cross-compiled to riscv32im and runs
*inside* the [RISC Zero](https://github.com/risc0/risc0) zkVM. The guest runs
a *job* — compile translation units to object files, and/or link objects into
a statically-linked x86-64 Linux ELF — entirely in an in-memory filesystem,
and commits a manifest to the journal:

```
ops (compile/link commands) || inputs [(path, sha256)..] || outputs [(path, sha256)..]
```

Receipts chain by hash equality: the link job's input object hashes must be
compile-job output hashes, so N parallel compile receipts + 1 link receipt
prove an entire build. Source files are linked to git commits at
*verification* time by content (resolve the path in a repo, compare the
blob's sha256) — same binding as proving a git hash, but free.

## The proof statement

> There exists an execution of the compiler image `IMAGE_ID` that, given
> exactly this input file tree (by sha256), ran these compile/link commands
> and produced these outputs (by sha256).

`IMAGE_ID` pins **everything**: the TCC binary (vendored at commit
`vendor/tinycc/VENDORED_COMMIT`, built with the flags in
`methods/guest/build.rs`), the compiler options (`-nostdlib` per TU,
`-static -nostdlib` for linking, baked into `methods/guest/shim/compile.c`),
the newlib syscall shims, and the Rust guest wrapper. Include paths and
`-D` defines are job inputs, but they are committed to the journal, so the
verifier sees them. Trusting a receipt means trusting (a) the RISC Zero proof
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

## Multi-TU builds: hello world against real musl

A build manifest (`build.json`) describes a file tree, translation units and
the final link. The host batches TUs into compile jobs (one receipt each,
embarrassingly parallel — `--only N` fans jobs out across machines), then
proves the link; `chain.json` ties the receipts together:

```sh
examples/musl-hello/prepare.sh     # pin musl v1.2.5, headers, TU closure (native tcc)
cargo run --release -p host -- --build examples/musl-hello/build.json --mode succinct
./out/hello                        # => hello, musl 42
cargo run --release -p verifier -- --chain out/chain.json \
    --git-dir . --git-dir examples/musl-hello/musl
```

That's 44 proven TUs (vfprintf and the stdio internals, memcpy/memset
x86-64 asm, TLS init, crt1, plus TCC's own runtime helpers from the vendor
tree) + a proven link = a real `printf` hello world, every byte of which
traces to pinned musl/tcc-verified git commits.

## Verifiable provenance — how to actually use this

A project ships provenance in three steps:

```sh
cloud/prove-remote.sh src/main.c                       # 1. prove (~cents; or --build m.json)
attest out/remote-main/receipt.groth16.bin \
       --exe out/remote-main/hello --embed             # 2. package + embed the attestation
git push && publish the binary                         # 3. sources reachable from public commits
```

A consumer verifies with **no toolchain, no rebuild, in milliseconds**:

```sh
verifier --exe ./hello --git-dir ./their-repo
# proofs             : 1 receipts VALID (5ms)
# binary sha256      : 2466bd... == ./hello ✓
# git linkage        : src/main.c -> ./their-repo@94da263 src/main.c ✓
```

What this proves: *this exact executable was produced by the pinned
compiler (image id in `CANONICAL_IMAGE_ID`) from source files whose
contents live at those paths in those commits.* The attestation
(`.prov.json`, or embedded in the executable as an ignored ELF trailer —
the binary still runs) contains the claims in the clear plus the 665-byte
Groth16 proof per receipt; every field is cross-checked against the proven
journals and the receipt chain, so nothing in the file can lie. The
verifier's trust root is the compiler pin baked in at build time — URLs
and descriptions inside attestations are unauthenticated hints, never
trust inputs. Full format and trust model:
[docs/provenance-design.md](docs/provenance-design.md).

## Documentation

- [docs/architecture.md](docs/architecture.md) — what/where/why/how for contributors
- [docs/provenance-design.md](docs/provenance-design.md) — attestation format, trust model, embedding
- [docs/measurements.md](docs/measurements.md) — all benchmark data + the cost model
- [docs/compiler-economics.md](docs/compiler-economics.md) — TCC vs gcc/clang, extrapolations
- [docs/operations.md](docs/operations.md) — cloud runbook, CI, sharp edges
- [docs/roadmap.md](docs/roadmap.md) — future plans

## Measurements

Compilation cost is linear in the bytes each TU parses (~450 guest
cycles/byte; job startup ≈ 1.2 M cycles; for multi-TU builds the per-TU
include closure dominates). Execution is ~25 MHz emulated — proving is the
entire cost. Receipt sizes and verification are constant: 223 KB / ~18 ms
(succinct), **665 B / ~5 ms (groth16)**, per receipt, any program size.
The musl hello chain is ~400 segments ⇒ ~10 min on one 4090 (~7¢), ~5 min
across 4 (see [docs/measurements.md](docs/measurements.md)).

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

- `examples/` — freestanding single-file programs (raw `syscall`, no libc):
  `hello.c`; realistic kernels `fib.c`, `primes.c` (sieve to 10⁶), `sort.c`
  (recursive quicksort + FNV self-check) whose proven binaries compute
  verifiably correct results, each ~12 s GPU proving; synthetic `bench_*.c`
  for size scaling; `multi-tu/` — minimal 2-TU chain; `musl-hello/` —
  `printf` hello against pinned musl v1.2.5 (44 proven TUs; `prepare.sh`
  computes the closure with a native build of the vendored tcc)
- `vendor/tinycc` — pinned TCC sources (see `VENDORED_COMMIT`);
  `lib/libtcc1.c`/`lib/va_list.c` double as provable runtime-helper TUs
- `jobfmt/` — shared job/journal/chain format (host ↔ guest ↔ verifier)
- `methods/guest/` — zkVM guest: `build.rs` cross-compiles TCC
  (`ONE_SOURCE`, `TCC_TARGET_X86_64`, `CONFIG_TCC_PREDEFS`), `shim/sys.c` is
  the newlib OS layer (bump-`sbrk`, in-memory filesystem, no real syscalls),
  `shim/compile.c` drives libtcc (compile-to-object and link ops),
  `src/main.rs` runs the job, hashes + commits the manifest
- `host/` — proves builds: single file or `--build build.json`
  (batched compile jobs + link job + `chain.json`; `--only` for fan-out)
- `verifier/` — standalone verification: every `receipt.verify(IMAGE_ID)`,
  the hash chain, binary hash, content-based git linkage

## Prior art

[CosmicTurtle (arXiv:2602.11887)](https://arxiv.org/abs/2602.11887) proved
chibicc compilations in RISC Zero, source → x86-64 *assembly text*, with
uncompressed composite receipts (MB-scale, slow verification). This PoC
differs in producing a *runnable linked ELF* inside the proof (TCC's
in-memory linker) and compressing to constant-size succinct/Groth16 receipts
that verify in milliseconds.
