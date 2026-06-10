# Architecture — what, where, why, how

For a future contributor/maintainer. Read this first.

## What

tcc-verified produces a **cryptographic proof that a binary is the result of
compiling a given C source with a pinned compiler**. TinyCC runs *inside* the
RISC Zero zkVM; the execution proof attests the compilation. Verification
needs no toolchain and no re-execution: a 665-byte Groth16 receipt checks in
~5 ms against a 32-byte image id.

## Why

Reproducible builds verify by re-execution (need the toolchain, determinism,
and patience); attestation schemes (SLSA, in-toto, TEE) verify signatures
(trust the signer/vendor). A zk proof of compilation removes trust in the
*builder* entirely — anyone can prove, nobody can forge. What remains is
trust in the *compiler itself* (Thompson's trusting-trust: the proof pins
the compiler binary, it cannot vouch for its honesty) and in the proof
system. See `docs/provenance-design.md` for the full trust model.

## The proof statement

> There exists an execution of guest image `IMAGE_ID` that read a source
> file with sha256 `H_src` (= git blob `B`) and produced output with
> sha256 `H_out`.

Journal layout (84 bytes): `sha256(src) ‖ git-blob-sha1(src) ‖ sha256(elf)`.

`IMAGE_ID` pins everything: the TCC binary (vendored commit in
`vendor/tinycc/VENDORED_COMMIT`), compiler flags (`-static -nostdlib`,
baked in `methods/guest/shim/compile.c`), the newlib syscall shims, the
Rust wrapper, and both lockfiles. Source files are runtime *inputs* — they
do not affect IMAGE_ID.

**The canonical IMAGE_ID comes from the CI-baked docker image, not from
local builds.** Guest builds are only path-reproducible inside a fixed
environment (`/opt/tcc-verified`, `/root/.cargo`); two independent builders
of the image produced identical ids. The pin lives in `CANONICAL_IMAGE_ID`
and CI fails the bake on drift.

## Where (repo map)

- `methods/guest/` — the zkVM guest ("the compiler"):
  - `build.rs` cross-compiles vendored TCC to riscv32im with the rzup C
    toolchain (`ONE_SOURCE`, `TCC_TARGET_X86_64`, `CONFIG_TCC_PREDEFS` so
    preprocessor defs are baked in as strings — no include files at runtime),
    and links a *filtered* newlib (members colliding with Rust's
    compiler-builtins/libm removed).
  - `shim/sys.c` — the entire "OS": bump-allocator `_sbrk`, one in-memory
    output file behind `_open/_write/_lseek`, `time()=0`, console capture.
  - `shim/compile.c` — drives libtcc: source string in, linked ELF out.
  - `src/main.rs` — hashes input/output, commits the journal.
- `host/` — proving driver. Modes: composite/succinct/groth16, `--dev`
  (fast iteration, fake receipts), `--wrap` (compress an existing succinct
  receipt to groth16 — pure compression, no id coupling), `--image-id`.
  `--features cuda` enables in-process GPU proving (`RISC0_PROVER=local`).
- `verifier/` — standalone verification CLI; embeds the canonical pin at
  build time. See `docs/provenance-design.md`.
- `vendor/tinycc` — pinned TCC sources. NOTE: tinycc's own `.gitignore`
  shadows files like `conftest.c`; vendor updates must `git add -f vendor/`.
- `cloud/` — vast.ai GPU offload (`prove-remote.sh`, `provision.sh`,
  `Dockerfile`). See `docs/operations.md`.
- `examples/` — freestanding C programs (no libc/includes — the statement
  is single-file `-nostdlib`).
- `.github/workflows/bake-prover-image.yml` — bakes the canonical image to
  ghcr, reports/enforces IMAGE_ID, cargo-chef + registry cache.

## How (data flow)

```
prove (local CPU or cloud GPU):
  host --mode succinct src.c
    └─ executor runs guest (TCC) at ~25 MHz emulated → binary + segments
    └─ STARK per 2^20-cycle segment → lift/join → succinct receipt (223 KB)
  host --wrap receipt → identity_p254 + rapidsnark (docker/podman)
    → groth16 receipt (665 B)

verify (anyone, anywhere):
  receipt.verify(CANONICAL_IMAGE_ID)  +  sha256(binary) == journal.h_out
  [optional] git blob/commit linkage: journal.blob_sha1 ∈ git tree
```

Key separations worth preserving:
- **execution ↔ proving**: compilation result is available at ~0.1% of the
  cost; proofs are generated after the fact and can be outsourced to
  untrusted GPUs (receipts are self-verifying — bad hosts can only fail to
  deliver, never forge).
- **proving ↔ wrapping**: the groth16 wrap is constant-cost (~160 s CPU)
  and runs anywhere podman/docker exists; cloud GPUs do only the
  size-linear succinct part.
- **compiler identity ↔ everything else**: only guest/toolchain changes
  move IMAGE_ID. Treat any change under `methods/`, `vendor/`, or the
  Dockerfile toolchain layers as a *compiler release* requiring a re-pin.
