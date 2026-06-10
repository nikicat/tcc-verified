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

## Measured on this machine (8-core i9-era CPU, AVX-512, no GPU)

Guest execution: ~1.34 M user cycles (2 segments) to compile
`examples/hello.c` (619 bytes) into a 1196-byte static ELF.

| receipt   | proof size | proving time | verification |
|-----------|-----------:|-------------:|-------------:|
| composite |     563 KB |       1101 s |        41 ms |
| succinct  |     223 KB |       1258 s |        18 ms |
| groth16   |  **665 B** |       1557 s |     **4 ms** |

(groth16 wraps the succinct pipeline plus a STARK→SNARK proving container —
run via rootless podman here. Proof size and verification cost are constant
in program size for succinct/groth16.)

Native `tcc examples/hello.c` compiles in ~1 ms; the ~10⁴–10⁵× proving
overhead is paid once, while every verification afterwards is milliseconds.

## Cloud GPU proving (vast.ai)

Proving is separable from compilation: execution runs at ~25 MHz emulated
(the 47 M-cycle benchmark executes in ~2 s) and segments serialize to
~250 KB each, so the heavy part — segment STARKs — can run anywhere. Receipts
are self-verifying, which makes the cheap spot GPU market safe: a bad host
can fail to deliver, but never forge.

```sh
pip install vastai && vastai set api-key <KEY>   # + ssh key in account, ~$10 balance
cloud/prove-remote.sh examples/bench_large.c     # rent 4090 -> prove -> fetch -> wrap -> verify
```

The driver rents the cheapest reliable RTX 4090 (~$0.35/hr), rsyncs the repo,
builds with `--features cuda` (first boot ~35 min, then incremental), proves
the **succinct** receipt on GPU, downloads it (223 KB), wraps to **groth16
locally** through podman (`host --wrap`, avoids docker-in-docker on container
rentals), verifies, and destroys the instance. IMAGE_ID is compared
local-vs-remote before proving (pinned rzup toolchains make guest builds
reproducible). `cloud/Dockerfile` bakes a prebuilt image for instant boots if
you push it to a registry.

Expected per-proof cost on a 4090: hello.c < 1¢, bench_large (~50 segments)
~5¢ in ~10 min — vs ~9 h on an 8-core CPU.

## Layout

- `examples/hello.c` — freestanding hello world (raw `syscall`, no libc), so
  the produced ELF depends on nothing outside the proof statement
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
