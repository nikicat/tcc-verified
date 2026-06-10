//! Package a groth16 receipt as a provenance attestation (.prov.json),
//! optionally embedding it into the executable as an appended trailer.
//!
//!   attest <receipt.groth16.bin> [--exe <path>] [--embed] [--out <file>]
//!
//! --embed rewrites <exe> in place as [exe][json][len][TCCPROV1]; the
//! binary still runs (ELF loaders ignore trailing bytes) and the verifier
//! strips the trailer before hashing. The .prov.json is also written
//! (default: <exe>.prov.json or <receipt>.prov.json).

#[path = "../attestation.rs"]
mod attestation;

use std::fs;

use attestation::{append_trailer, hex, split_trailer, Attestation, Claims, CompilerInfo, ProofInfo, FORMAT};
use base64::Engine as _;
use risc0_zkvm::Receipt;

fn main() {
    let mut args: Vec<String> = std::env::args().skip(1).collect();
    let mut take_opt = |name: &str| -> Option<String> {
        args.iter().position(|a| a == name).map(|i| {
            args.remove(i);
            args.remove(i)
        })
    };
    let exe = take_opt("--exe");
    let out = take_opt("--out");
    let embed = args.iter().position(|a| a == "--embed").map(|i| args.remove(i)).is_some();
    let [receipt_path] = args.as_slice() else {
        eprintln!("usage: attest <receipt.groth16.bin> [--exe <path>] [--embed] [--out <file.prov.json>]");
        std::process::exit(2);
    };

    let pin_hex = env!("CANONICAL_IMAGE_ID_HEX");
    assert!(!pin_hex.is_empty(), "built without CANONICAL_IMAGE_ID");

    let receipt_bytes = fs::read(receipt_path).expect("read receipt");
    let receipt: Receipt = bincode::deserialize(&receipt_bytes).expect("decode receipt");
    let j = &receipt.journal.bytes;
    assert_eq!(j.len(), 84, "unexpected journal size");

    let att = Attestation {
        format: FORMAT.into(),
        claims: Claims {
            source_sha256: hex(&j[0..32]),
            source_git_blob_sha1: hex(&j[32..52]),
            binary_sha256: hex(&j[52..84]),
        },
        compiler: CompilerInfo {
            image_id: pin_hex.into(),
            description: "TinyCC b8f680a3 (-static -nostdlib) in RISC Zero zkVM 3.0.5".into(),
            project: "https://github.com/nikicat/tcc-verified".into(),
        },
        proof: ProofInfo {
            system: "risc0".into(),
            risc0_version: "3.0.5".into(),
            receipt_bincode_b64: base64::engine::general_purpose::STANDARD.encode(&receipt_bytes),
        },
    };
    let json = serde_json::to_vec_pretty(&att).unwrap();

    let out_path = out.unwrap_or_else(|| {
        format!("{}.prov.json", exe.as_deref().unwrap_or(receipt_path.trim_end_matches(".bin")))
    });
    fs::write(&out_path, &json).unwrap();
    println!("attestation        : {out_path} ({} bytes)", json.len());

    if embed {
        let exe_path = exe.expect("--embed requires --exe");
        let file = fs::read(&exe_path).expect("read exe");
        let (orig, old) = split_trailer(&file);
        if old.is_some() {
            println!("note               : replacing existing trailer");
        }
        // sanity: only embed into the binary this receipt attests
        let h: [u8; 32] = <sha2::Sha256 as sha2::Digest>::digest(orig).into();
        assert_eq!(hex(&h), att.claims.binary_sha256, "--exe does not match the receipt's binary hash");
        let perms = fs::metadata(&exe_path).unwrap().permissions();
        fs::write(&exe_path, append_trailer(orig, &json)).unwrap();
        fs::set_permissions(&exe_path, perms).unwrap();
        println!("embedded           : {exe_path} (binary still runnable; verifier strips the trailer)");
    }
}
