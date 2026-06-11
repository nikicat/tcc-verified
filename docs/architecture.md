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

One guest execution = one *job* = one receipt. A job is a file tree plus a
list of ops: compile a TU to an ELF object (`-nostdlib`), or link objects
into a static executable (`-static -nostdlib`).

> There exists an execution of guest image `IMAGE_ID` that, given exactly
> this input file tree (by sha256), ran these ops and produced these
> outputs (by sha256).

Journal v2 (`jobfmt::Manifest`, self-describing bytes, `"TCV2"` magic):
the ops (with their include dirs and `-D` defines — path resolution is a
*committed* job input, flags are baked), `[(path, sha256)]` for every input
file, `[(path, sha256)]` for every output.

Receipts **chain**: `jobfmt::verify_chain` requires every object the link
op consumes to be a compile-op output (by hash) in some receipt of the
chain, exactly one link op, and consistent contents per path across jobs.
N compile jobs (batched TUs, parallelizable) + 1 link job prove a build.
A single-file build is the same machinery with one job (compile + link).

Git linkage moved *out* of the proof: instead of an in-guest git-blob-sha1
(software SHA-1 would cost ~20 cycles/byte over the whole header tree per
job), the verifier resolves each proven source path in `--git-dir` repos
and compares the blob content's sha256. Same binding, zero proof cost,
and it works across multiple repos (musl's sources link to musl.git,
TCC's runtime helpers to this repo).

`IMAGE_ID` pins everything: the TCC binary (vendored commit in
`vendor/tinycc/VENDORED_COMMIT`), compiler flags (baked in
`methods/guest/shim/compile.c`), the newlib syscall shims, the in-memory
filesystem, the Rust wrapper, and both lockfiles. Source files, include
paths and defines are runtime *inputs* — they do not affect IMAGE_ID.

**The canonical IMAGE_ID comes from the CI-baked docker image, not from
local builds.** Guest builds are only path-reproducible inside a fixed
environment (`/opt/tcc-verified`, `/root/.cargo`); two independent builders
of the image produced identical ids. The pin lives in `CANONICAL_IMAGE_ID`
and CI fails the bake on drift.

## Where (repo map)

- `jobfmt/` — shared vocabulary (no deps beyond serde-free hand encoding):
  `Job`/`Op` host→guest framing, journal v2 `Manifest` encode/decode, and
  `verify_chain` — the pure hash-chain logic both host and verifier run.
- `methods/guest/` — the zkVM guest ("the compiler"):
  - `build.rs` cross-compiles vendored TCC to riscv32im with the rzup C
    toolchain (`ONE_SOURCE`, `TCC_TARGET_X86_64`, `CONFIG_TCC_PREDEFS` so
    preprocessor defs are baked in as strings — no include files at runtime),
    and links a *filtered* newlib (members colliding with Rust's
    compiler-builtins/libm removed).
  - `shim/sys.c` — the entire "OS": bump-allocator `_sbrk`, an in-memory
    filesystem (input files registered from Rust, outputs in a static
    arena, lexical `..` normalization for musl-style relative includes),
    `time()=0`, console capture.
  - `shim/compile.c` — drives libtcc, one fresh TCCState per op:
    `zk_compile_obj` (TU → .o) and `zk_link` (.o's → static ELF).
  - `src/main.rs` — reads the job, registers files, runs ops, hashes
    everything, commits the manifest, streams outputs to the host.
- `host/` — proving driver. Single-file mode (one compile+link job) or
  `--build build.json` (batched compile jobs + link job + `chain.json`;
  `--only N|link` to fan jobs out across machines). Modes:
  composite/succinct/groth16, `--dev` (fast iteration, fake receipts),
  `--wrap` (compress an existing succinct receipt to groth16 — pure
  compression, no id coupling), `--image-id`. `--features cuda` enables
  in-process GPU proving (`RISC0_PROVER=local`).
- `verifier/` — standalone verification CLI (`--chain`, `--exe`
  + attestation, legacy raw mode); embeds the canonical pin at build time.
  See `docs/provenance-design.md`.
- `vendor/tinycc` — pinned TCC sources. NOTE: tinycc's own `.gitignore`
  shadows files like `conftest.c`; vendor updates must `git add -f vendor/`.
  `lib/{libtcc1,va_list}.c` double as *provable TUs*: `-nostdlib` builds
  that need TCC's runtime helpers compile them like any other source.
- `cloud/` — vast.ai GPU offload (`prove-remote.sh`, also `--build`;
  `provision.sh`, `Dockerfile`). See `docs/operations.md`.
- `examples/` — freestanding single-file programs (`hello.c`, kernels,
  `bench_*.c`); `multi-tu/` (minimal 2-TU + header chain);
  `musl-hello/` (prepare.sh pins musl v1.2.5, computes the TU closure with
  a native build of the vendored tcc, emits build.json — 44 TUs).
- `.github/workflows/bake-prover-image.yml` — bakes the canonical image to
  ghcr, reports/enforces IMAGE_ID, cargo-chef + registry cache.

## How (data flow)

```
prove (local CPU or cloud GPU):
  host src.c | host --build build.json     (TU batches -> compile jobs)
    └─ per job: executor runs guest (TCC) at ~25 MHz emulated
       → .o files / linked ELF + segments
    └─ STARK per 2^20-cycle segment → lift/join → succinct receipt (223 KB)
  host --wrap receipt → identity_p254 + rapidsnark (docker/podman)
    → groth16 receipt (665 B)               (one wrap per receipt)

verify (anyone, anywhere):
  every receipt.verify(CANONICAL_IMAGE_ID)
  + verify_chain: link-op object hashes ⊆ compile-op output hashes,
    sha256(binary) == link output hash
  [optional] git linkage: resolve each source path in --git-dir repo(s),
    sha256(blob content) == proven input hash
```

Key separations worth preserving:
- **execution ↔ proving**: compilation result is available at ~0.1% of the
  cost; proofs are generated after the fact and can be outsourced to
  untrusted GPUs (receipts are self-verifying — bad hosts can only fail to
  deliver, never forge).
- **proving ↔ wrapping**: the groth16 wrap is constant-cost (~160 s CPU)
  and runs anywhere podman/docker exists; cloud GPUs do only the
  size-linear succinct part.
- **compile ↔ link**: receipts chain by hash, so per-batch compile
  receipts are independent (parallel across machines via `host --only`,
  cacheable across builds); a changed TU costs one batch + the link
  re-proof, not the world.
- **compiler identity ↔ everything else**: only guest/toolchain changes
  move IMAGE_ID. Treat any change under `methods/`, `vendor/`, or the
  Dockerfile toolchain layers as a *compiler release* requiring a re-pin.
