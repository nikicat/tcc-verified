# Provenance design — attestation format, trust model, embedding

## Vocabulary

"Receipt" is RISC Zero's name for the raw proof object. The distribution
artifact this project ships is a **provenance attestation** (`.prov.json`)
— aligned with SLSA/in-toto vocabulary, except the backing is a zk proof
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
proof verifies against the pin. URLs, descriptions, claimed image ids:
unauthenticated hints for humans and tooling. The verifier MUST compare
the attestation's image id against its own pin and MUST cross-check the
cleartext claims against the proven journal — both implemented in
`verifier`.

## Attestation file format (`tcc-verified-attestation/1`)

```json
{
  "format": "tcc-verified-attestation/1",
  "claims": {
    "source_sha256": "<hex32>",
    "source_git_blob_sha1": "<hex20>",
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
    "receipt_bincode_b64": "<base64>"
  }
}
```

Design choices:
- `claims` duplicates the journal **in the clear** for tooling/humans; the
  verifier recomputes them from the proven journal and rejects mismatches,
  so the duplication cannot lie.
- The embedded receipt stays in risc0's native bincode (wrapped in b64):
  re-encoding the Groth16 seal into a stable independent format is future
  work tracked in the roadmap; pinning `risc0_version` makes the coupling
  explicit.
- `compiler.project` is a hint. Implementations MUST NOT fetch the pin
  from it at verification time.

Created by `attest` (verifier crate): wraps a `receipt.groth16.bin` into
the JSON envelope, optionally embeds into the executable.

## Embedding in the executable

The proof cannot live *inside* the attested bytes — the journal commits
sha256(B), so B cannot contain its own proof (circularity). Two standard
escapes:

1. **Appended trailer (implemented).** ELF loaders ignore bytes after the
   image, so we append `[attestation json][u32 LE length]["TCCPROV1"]`.
   The verifier sees the magic at EOF, splits the file, hashes only the
   original bytes. Byte-exact, no objcopy canonicalization risk, the
   binary still runs. Same "hash everything but the signature blob"
   pattern as Authenticode/macOS code signing.
2. ELF section (`.note.provenance`) stripped before hashing — rejected
   for now: objcopy round-trips are not reliably byte-stable.

## Verification (the `verifier` CLI)

```
verifier --exe <path> --attestation <file.prov.json> [--git-dir <repo>]
verifier --exe <path-with-embedded-trailer> [--git-dir <repo>]
```

Steps: parse attestation (from file or trailer) → check claimed image id
== pin (embedded at build from `CANONICAL_IMAGE_ID`; `--image-id`
overrides) → cryptographically verify receipt → sha256(exe minus trailer)
== proven `binary_sha256` → cleartext claims == journal → if `--git-dir`:
the proven blob sha1 exists in the repo's object store, and its paths in
HEAD's tree are reported (commit → tree → blob is git's own merkle chain).
Exit 0 on success, non-zero with a specific error otherwise.

## How a project ships verifiable provenance

1. Build & prove: `cloud/prove-remote.sh src.c` (or local `host`).
2. Package: `attest out/receipt.groth16.bin --exe out/hello --embed`
   (or ship `hello.prov.json` next to the release artifact).
3. Publish the artifact; commit the source so the blob is reachable from
   a public commit.
4. Consumers verify with the 3-line command above against the pin they
   already have. No toolchain, no rebuild, ~5 ms.
