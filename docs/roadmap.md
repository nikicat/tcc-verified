# Roadmap

Roughly ordered; items are independent unless noted.

Done 2026-06-11 (was the top of this list): **multi-file inputs** (in-guest
memfs, journal v2 manifests), **multi-TU pipeline** (batched compile jobs +
proven link, receipts chained by object hashes, `--only` fan-out), and
**hello-world against real musl** (examples/musl-hello: 44 proven TUs,
git-linked to musl.git by content). New IMAGE_ID — needs a re-pin bake.

## Near-term (each ~a day)

- **Prove musl-hello on GPU end-to-end**: rebake the canonical image
  (~400 segments ⇒ ~10 min single 4090, ~7¢; `prove-remote.sh --build` is
  written but not yet exercised on a rental). Publish the attestation.
- **Full musl libc.a**: all ~1262 TUs as a cached receipt set. Blocker: 37
  TUs fail TCC (x87 asm constraints `x`/`t`, `expl.s` syntax) — either
  upstream-style tcc patches, C fallbacks, or excluding the long-double
  math that needs them. ~$1.40 serial, parallelizes linearly.
- **DOOM**: build manifest for doomgeneric + the musl closure it needs;
  multi-file inputs no longer block it (see measurements extrapolations).
- **sqlite amalgamation stress test**: bump the guest C heap (currently
  64 MB static), find the practical single-TU ceiling vs the rv32 address
  space.
- **Example/sidecar repo**: a hello-world repo whose release binary anyone
  verifies with three command-line arguments (see provenance-design.md
  "how a project ships").

## Medium-term

- **Manifest compression**: journals now carry the full input file list
  (~10–35 KB per musl-hello job, headers repeated per batch). Commit a
  merkle root instead — most naturally the **git tree hash**, which also
  upgrades linkage to "this commit's whole tree". New IMAGE_ID.
- **Incremental proving**: receipts are content-addressed by the manifest;
  cache compile receipts keyed on (input hashes, ops) and only re-prove
  changed batches + the link.
- **Parallel segment dispatcher**: ship segments (~250 KB each) to several
  GPU boxes and join remotely; only worth it past ~50 segments/program.
  (Multi-TU batching already gives job-level parallelism — this is the
  finer-grained version.)
- **Stable attestation encoding**: re-encode the Groth16 seal + journal in
  a risc0-version-independent format (the seal is just curve points), so
  attestations outlive risc0's bincode layout.
- **Second compiler image: cproc/QBE** — real optimization at ~5–10× TCC
  cost (compiler-economics.md). Registry of canonical compiler ids
  (id → compiler description) instead of a single pin.
- **Boundless/Bonsai backend**: prove via RISC Zero's market instead of
  managed instances; zero infra, pay per proof.

## Long-term / research

- **gcc-scale compilers**: blocked on 64-bit-guest zkVMs (memory, not
  cycles). Watch rv64 guests; revisit when address space > 4 GB.
- **Proven bootstrap chains**: TCC (proven) compiles a bigger compiler
  (proven) compiles the target — composable receipts attacking
  trusting-trust step by step.
- **Package-manager integration**: a pacman/apt hook verifying embedded
  attestations at install time; distro rebuilders publishing attestations
  alongside packages.
- **Proof composition**: aggregate the per-batch receipt chain into one
  release-level receipt (risc0 supports receipt composition in-guest) —
  collapses multi-receipt attestations back to a single constant-size
  proof and moves `verify_chain` inside the proven statement.

## Maintenance notes

- risc0 upgrades change proving performance and possibly receipt formats:
  re-measure, re-pin (new IMAGE_ID), keep old pins listed for verifying
  historical attestations.
- TCC vendor bumps: `git add -f vendor/`, update VENDORED_COMMIT, re-pin.
- The 2 dependabot alerts (low) on the repo are in dev-path dependencies;
  triage when convenient.
