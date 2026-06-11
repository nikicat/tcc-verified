# Provenance design — attestation format, trust model, embedding

## Vocabulary

"Receipt" is RISC Zero's name for the raw proof object. A build is proven by
a *chain* of receipts: N compile jobs (batches of translation units, each
producing object files) plus one link job whose inputs must hash-match the
compile outputs. A single-file build is the degenerate one-receipt chain
(compile + link in one job). The distribution artifact this project ships is
a **provenance attestation** (`.prov.json`) bundling the whole chain —
aligned with SLSA/in-toto vocabulary, except the backing is a zk proof
rather than a builder's signature. The crucial upgrade over signature-based
provenance: *nobody* needs to be trusted to have built honestly; the claim
is checkable, not asserted.

## Trust model (read this before anything else)

A verifier trusts exactly three things:

1. **The pinned compiler id** (`CANONICAL_IMAGE_ID`) — obtained
   out-of-band from a source the verifier chooses to trust (this repo, its
   CI, or their own rebuild of the docker image). This is the trust root,
   exactly like certificate pinning.
2. **The proof system** (RISC Zero Groth16 verifier + its ceremony).
3. **That the pinned compiler is an honest compiler** — the proof pins
   *which* compiler ran; it cannot vouch for the compiler's semantics
   (Thompson's trusting-trust). Mitigation: the compiler sources are
   vendored, pinned, auditable, and the image build is reproducible.

Everything *inside* an attestation file is attacker-controlled until the
proofs verify against the pin. URLs, descriptions, claimed image ids:
unauthenticated hints for humans and tooling. The verifier MUST compare
the attestation's image id against its own pin, MUST verify every receipt,
MUST run the chain check (`jobfmt::verify_chain`: exactly one link op,
every linked object is a proven compile output by hash, no conflicting
contents per path), and MUST cross-check the cleartext claims against the
proven journals — all implemented in `verifier`.

Include paths and `-D` defines are proven journal content (they steer path
resolution inside the committed input tree); the compiler *flags* are baked
into the image. A consumer who cares can read the ops out of the journals.

## Attestation file format (`tcc-verified-attestation/2`)

```json
{
  "format": "tcc-verified-attestation/2",
  "claims": {
    "sources": [ {"path": "src/stdio/printf.c", "sha256": "<hex32>"}, ... ],
    "binary_sha256": "<hex32>"
  },
  "compiler": {
    "image_id": "<hex32>",
    "description": "TinyCC b8f680a3 (-static -nostdlib) in RISC Zero zkVM",
    "project": "https://github.com/nikicat/tcc-verified"
  },
  "proof": {
    "system": "risc0",
    "risc0_version": "3.0.5",
    "receipts_bincode_b64": ["<base64>", ...]
  }
}
```

Design choices:
- `claims` duplicates the proven chain summary **in the clear** for
  tooling/humans; the verifier recomputes it from the proven journals and
  rejects mismatches, so the duplication cannot lie.
- `claims.sources` is every input file the binary derives from (sources
  *and* headers), deduped across compile jobs — the full input tree, not
  just the TUs.
- The embedded receipts stay in risc0's native bincode (wrapped in b64):
  re-encoding the Groth16 seal into a stable independent format is future
  work tracked in the roadmap; pinning `risc0_version` makes the coupling
  explicit.
- `compiler.project` is a hint. Implementations MUST NOT fetch the pin
  from it at verification time.

Created by `attest` (verifier crate) from a `chain.json` (multi-TU) or a
single receipt file; optionally embedded into the executable.

## Git linkage (by content, at verification time)

The journal commits `(path, sha256)` per input file — no git hash is proven
in-guest (SHA-1 in software would cost ~20 cycles/byte across every job's
header tree). Instead, `verifier --git-dir <repo>` (repeatable) resolves
each proven path in the repo's HEAD tree — exact path first, then suffix
match — reads the blob, and compares the *content's* sha256 against the
proven hash. That is the same binding (git blobs are content-addressed),
costs nothing inside the proof, and naturally spans multiple repos: in
`examples/musl-hello`, musl sources link to musl.git's pinned commit and
TCC's runtime-helper TUs link to this repo, in one invocation.

Files that match no repo (e.g. musl's two sed-generated headers) are
listed explicitly; linkage is a report for the consumer to audit, while
the cryptographic verdict (proofs + chain + binary hash) drives the exit
code.

## Embedding in the executable

The proof cannot live *inside* the attested bytes — the journal commits
sha256(B), so B cannot contain its own proof (circularity). Two standard
escapes:

1. **Appended trailer (implemented).** ELF loaders ignore bytes after the
   image, so we append `[attestation json][u32 LE length]["TCCPROV2"]`.
   The verifier sees the magic at EOF, splits the file, hashes only the
   original bytes. Byte-exact, no objcopy canonicalization risk, the
   binary still runs. Same "hash everything but the signature blob"
   pattern as Authenticode/macOS code signing.
2. ELF section (`.note.provenance`) stripped before hashing — rejected
   for now: objcopy round-trips are not reliably byte-stable.

## Verification (the `verifier` CLI)

```
verifier --chain <chain.json> [--exe <path>] [--git-dir <repo>]...
verifier --exe <path> [--attestation <file.prov.json>] [--git-dir <repo>]...
verifier <receipt.bin> <binary> [source.c]              (legacy/raw)
```

Steps: parse chain/attestation (from file or trailer) → check claimed image
id == pin (embedded at build from `CANONICAL_IMAGE_ID`; `--image-id`
overrides) → cryptographically verify every receipt → chain check →
sha256(exe minus trailer) == proven binary hash → cleartext claims ==
proven journals → git linkage report. Exit 0 on success, non-zero with a
specific error otherwise.

## How a project ships verifiable provenance

1. Build & prove: `cloud/prove-remote.sh --build build.json` (or local
   `host --build`; single files: `cloud/prove-remote.sh src.c`).
2. Package: `attest out/chain.json --exe out/hello --embed`
   (or ship `hello.prov.json` next to the release artifact).
3. Publish the artifact; commit the sources so the blobs are reachable
   from public commits (upstream repos count — musl's own history serves
   musl's files).
4. Consumers verify with one command against the pin they already have.
   No toolchain, no rebuild, milliseconds per receipt.
