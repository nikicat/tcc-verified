//! Package a receipt chain as a provenance attestation (.prov.json),
//! optionally embedding it into the executable as an appended trailer.
//!
//!   attest <chain.json|receipt.bin> [--exe <path>] [--embed] [--out <file>]
//!
//! Accepts either a multi-TU chain.json (bundles all its receipts) or a
//! single receipt file (one-receipt chain). The claims are derived from the
//! receipts' journals via the same chain logic the verifier uses.
//!
//! --embed rewrites <exe> in place as [exe][json][len][TCCPROV2]; the
//! binary still runs (ELF loaders ignore trailing bytes) and the verifier
//! strips the trailer before hashing. The .prov.json is also written
//! (default: <exe>.prov.json or <receipt>.prov.json).

#[path = "../attestation.rs"]
mod attestation;

use std::fs;
use std::path::Path;

use attestation::{
    append_trailer, hex, split_trailer, Attestation, Claims, CompilerInfo, ProofInfo, SourceClaim,
    FORMAT,
};
use base64::Engine as _;
use jobfmt::Manifest;
use risc0_zkvm::Receipt;

#[derive(serde::Deserialize)]
struct ChainFile {
    format: String,
    receipts: Vec<String>,
}

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
    let [input_path] = args.as_slice() else {
        eprintln!("usage: attest <chain.json|receipt.bin> [--exe <path>] [--embed] [--out <file.prov.json>]");
        std::process::exit(2);
    };

    let pin_hex = env!("CANONICAL_IMAGE_ID_HEX");
    assert!(!pin_hex.is_empty(), "built without CANONICAL_IMAGE_ID");

    // load the receipt chain (a single receipt is a one-element chain)
    let receipt_blobs: Vec<Vec<u8>> = if input_path.ends_with(".json") {
        let dir = Path::new(input_path).parent().unwrap_or(Path::new("."));
        let chain: ChainFile =
            serde_json::from_slice(&fs::read(input_path).expect("read chain.json"))
                .expect("parse chain.json");
        assert_eq!(chain.format, "tcc-verified-chain/1", "unsupported chain format");
        chain.receipts.iter().map(|f| fs::read(dir.join(f)).expect("read receipt")).collect()
    } else {
        vec![fs::read(input_path).expect("read receipt")]
    };

    // derive claims from the journals (the verifier re-derives and
    // cross-checks these, so they cannot lie)
    let manifests: Vec<Manifest> = receipt_blobs
        .iter()
        .map(|b| {
            let r: Receipt = bincode::deserialize(b).expect("decode receipt");
            Manifest::decode(&r.journal.bytes).expect("decode journal")
        })
        .collect();
    let summary = jobfmt::verify_chain(&manifests).expect("receipts do not form a valid chain");

    let att = Attestation {
        format: FORMAT.into(),
        claims: Claims {
            sources: summary
                .sources
                .iter()
                .map(|f| SourceClaim { path: f.path.clone(), sha256: hex(&f.sha256) })
                .collect(),
            binary_sha256: hex(&summary.executable.sha256),
        },
        compiler: CompilerInfo {
            image_id: pin_hex.into(),
            description: "TinyCC b8f680a3 (-static -nostdlib) in RISC Zero zkVM 3.0.5".into(),
            project: "https://github.com/nikicat/tcc-verified".into(),
        },
        proof: ProofInfo {
            system: "risc0".into(),
            risc0_version: "3.0.5".into(),
            receipts_bincode_b64: receipt_blobs
                .iter()
                .map(|b| base64::engine::general_purpose::STANDARD.encode(b))
                .collect(),
        },
    };
    let json = serde_json::to_vec_pretty(&att).unwrap();

    let out_path = out.unwrap_or_else(|| {
        format!("{}.prov.json", exe.as_deref().unwrap_or(input_path.trim_end_matches(".bin")))
    });
    fs::write(&out_path, &json).unwrap();
    println!(
        "attestation        : {out_path} ({} bytes, {} receipts, {} sources)",
        json.len(),
        receipt_blobs.len(),
        att.claims.sources.len()
    );

    if embed {
        let exe_path = exe.expect("--embed requires --exe");
        let file = fs::read(&exe_path).expect("read exe");
        let (orig, old) = split_trailer(&file);
        if old.is_some() {
            println!("note               : replacing existing trailer");
        }
        // sanity: only embed into the binary this chain attests
        let h: [u8; 32] = <sha2::Sha256 as sha2::Digest>::digest(orig).into();
        assert_eq!(hex(&h), att.claims.binary_sha256, "--exe does not match the proven binary hash");
        let perms = fs::metadata(&exe_path).unwrap().permissions();
        fs::write(&exe_path, append_trailer(orig, &json)).unwrap();
        fs::set_permissions(&exe_path, perms).unwrap();
        println!("embedded           : {exe_path} (binary still runnable; verifier strips the trailer)");
    }
}
