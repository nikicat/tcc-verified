//! The provenance attestation format (`tcc-verified-attestation/1`) and the
//! executable trailer encoding. See docs/provenance-design.md.
//!
//! Trust note: everything in an attestation is an unauthenticated hint until
//! the embedded proof verifies against the verifier's own pinned image id.

use serde::{Deserialize, Serialize};

pub const FORMAT: &str = "tcc-verified-attestation/1";
pub const TRAILER_MAGIC: &[u8; 8] = b"TCCPROV1";

#[derive(Serialize, Deserialize)]
pub struct Attestation {
    pub format: String,
    pub claims: Claims,
    pub compiler: CompilerInfo,
    pub proof: ProofInfo,
}

#[derive(Serialize, Deserialize)]
pub struct Claims {
    pub source_sha256: String,
    pub source_git_blob_sha1: String,
    pub binary_sha256: String,
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
    pub receipt_bincode_b64: String,
}

pub fn hex(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02x}")).collect()
}

/// Split a file that may carry an appended attestation trailer:
/// [original bytes][attestation json][u32 LE json length][b"TCCPROV1"].
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
