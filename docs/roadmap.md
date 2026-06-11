# Roadmap

Roughly ordered; items are independent unless noted.

Done 2026-06-11 (was the top of this list): **multi-file inputs** (in-guest
memfs, journal v2 manifests), **multi-TU pipeline** (batched compile jobs +
proven link, receipts chained by object hashes, `--only` fan-out), and
**hello-world against real musl, proven end-to-end on a rented 4090**
(examples/musl-hello: 44 TUs, 740 s GPU, ~12¢, attestation embedded in the
binary, 113/115 sources git-linked by content). IMAGE_ID re-pinned to
`a2860a62` (local canonical-env bake ×2; CI bake must reproduce it).

## Near-term (each ~a day)
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

- **Prebuilt musl in the image**: prove musl once with the multi-TU chain,
  then bake the resulting headers + `libc.a` (+ `crt1.o`) into the guest —
  they join the IMAGE_ID pin (a "compiler+libc release"), and per-program
  cost drops to the user's TUs + link (headers still ship as inputs and
  set the cost floor: a printf hello ≈ 2–3 segments). Static linking
  against the baked archive keeps today's self-contained binaries;
  dynamic (`libc.so` + baked `-dynamic-linker /lib/ld-musl-x86_64.so.1`)
  also works in TCC and shrinks binaries, at the price of a runtime
  dependency on an unproven-at-runtime `libc.so`. Trust note: this moves
  libc inside the pinned TCB — mitigated by publishing the baked libc's
  own receipt chain (proven bootstrap, see long-term) and the
  reproducible image build.

## Medium-term

- **Attestation to a git commit** ("this exe was built from signed commit
  C by builder B"): add a *tree-attestation receipt* — one dedicated job
  per build that reads the input tree and commits its **git tree hash**
  (in-guest SHA-1 over blobs+trees costs ~20 cycles/byte *once per
  build*, not per batch — the reason per-file linkage went content-based)
  plus the per-file sha256 manifest. Compile jobs keep binding per-file
  sha256; `verify_chain` additionally requires compile inputs ⊆ tree
  receipt's file set. Upgrades the statement from "exe ← these listed
  files" to "exe ← exactly commit C's tree": a signed commit (the
  signature covers the commit object, which contains the tree hash) then
  yields "built from what the signer signed". Pairs with **signed
  builders**: the verifier accepts a trusted party's signature over the
  32-byte guest IMAGE_ID instead of (or alongside) the compiled-in pin —
  the compiler-id registry below, with signatures. Caveat: generated
  files (musl's alltypes.h/syscall.h) break tree equality — commit them,
  prove the generation step, or list them as explicit audited exceptions
  in the attestation. Also subsumes **manifest compression** (journals
  shrink to the tree hash + output hashes). New IMAGE_ID.
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
