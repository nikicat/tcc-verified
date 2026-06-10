# Roadmap

Roughly ordered; items are independent unless noted.

## Near-term (each ~a day)

- **Multi-file inputs**: populate the guest memfs with a file tree; journal
  commits a merkle root over inputs — most naturally the **git tree hash**,
  upgrading provenance from "this blob" to "this commit's whole tree"
  while keeping the existing commit-linkage story. New IMAGE_ID.
- **musl headers + libc.a in the guest**: enables `#include <...>` and
  linking real programs. Combined with multi-file inputs unlocks DOOM
  (~20 min GPU, ~12¢ — see measurements.md extrapolations). New IMAGE_ID.
- **sqlite amalgamation stress test**: bump the guest C heap (currently
  64 MB static), find the practical single-TU ceiling vs the rv32 address
  space.
- **Example/sidecar repo**: a hello-world repo whose release binary anyone
  verifies with three command-line arguments (see provenance-design.md
  "how a project ships").

## Medium-term

- **Multi-TU pipeline**: N proofs (one per TU, embarrassingly parallel
  across GPU instances) + a proven link step (TCC links .o files too), or
  one sequential execution compiling all TUs. Target: musl end-to-end
  (~45¢, minutes of wall time on a small fleet).
- **Parallel segment dispatcher**: ship segments (~250 KB each) to several
  GPU boxes and join remotely; only worth it past ~50 segments/program.
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
- **Proof composition**: aggregate per-TU receipts into one release-level
  receipt (risc0 supports receipt composition in-guest).

## Maintenance notes

- risc0 upgrades change proving performance and possibly receipt formats:
  re-measure, re-pin (new IMAGE_ID), keep old pins listed for verifying
  historical attestations.
- TCC vendor bumps: `git add -f vendor/`, update VENDORED_COMMIT, re-pin.
- The 2 dependabot alerts (low) on the repo are in dev-path dependencies;
  triage when convenient.
