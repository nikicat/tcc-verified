//! The provenance attestation format (`tcc-verified-attestation/2`) and the
//! executable trailer encoding. See docs/provenance-design.md.
//!
//! v2 carries a *chain* of receipts (N compile jobs + 1 link job; a
//! single-file build is the degenerate one-receipt chain) and claims the
//! source tree as a list of (path, sha256). Git linkage is established at
//! verification time by content (resolve path in a repo, compare sha256 of
//! the blob), so no git hash needs to be proven in-guest.
//!
//! Trust note: everything in an attestation is an unauthenticated hint until
//! the embedded proofs verify against the verifier's own pinned image id.

use serde::{Deserialize, Serialize};

pub const FORMAT: &str = "tcc-verified-attestation/2";
pub const TRAILER_MAGIC: &[u8; 8] = b"TCCPROV2";

#[derive(Serialize, Deserialize)]
pub struct Attestation {
    pub format: String,
    pub claims: Claims,
    pub compiler: CompilerInfo,
    pub proof: ProofInfo,
}

#[derive(Serialize, Deserialize)]
pub struct Claims {
    /// Every input file the executable provably derives from (sources,
    /// headers), deduped across compile jobs.
    pub sources: Vec<SourceClaim>,
    pub binary_sha256: String,
}

#[derive(Serialize, Deserialize, PartialEq, Eq)]
pub struct SourceClaim {
    pub path: String,
    pub sha256: String,
}

#[derive(Serialize, Deserialize)]
pub struct CompilerInfo {
    pub image_id: String,
    pub description: String,
    pub project: String,
}

#[derive(Serialize, Deserialize)]
pub struct ProofInfo {
    pub system: String,
    pub risc0_version: String,
    /// The receipt chain: N compile-job receipts + the link receipt, each
    /// risc0 bincode, base64. Order is irrelevant to verification.
    pub receipts_bincode_b64: Vec<String>,
}

pub fn hex(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02x}")).collect()
}

/// Split a file that may carry an appended attestation trailer:
/// [original bytes][attestation json][u32 LE json length][b"TCCPROV2"].
/// Returns (original bytes, attestation json) — the original bytes are what
/// the proven binary_sha256 covers; ELF loaders ignore the trailer.
pub fn split_trailer(file: &[u8]) -> (&[u8], Option<&[u8]>) {
    let n = file.len();
    if n < 12 || &file[n - 8..] != TRAILER_MAGIC {
        return (file, None);
    }
    let len = u32::from_le_bytes(file[n - 12..n - 8].try_into().unwrap()) as usize;
    if len + 12 > n {
        return (file, None);
    }
    (&file[..n - 12 - len], Some(&file[n - 12 - len..n - 12]))
}

/// Append an attestation trailer to executable bytes.
pub fn append_trailer(exe: &[u8], attestation_json: &[u8]) -> Vec<u8> {
    let mut out = Vec::with_capacity(exe.len() + attestation_json.len() + 12);
    out.extend_from_slice(exe);
    out.extend_from_slice(attestation_json);
    out.extend_from_slice(&(attestation_json.len() as u32).to_le_bytes());
    out.extend_from_slice(TRAILER_MAGIC);
    out
}
