//! Standalone verifier: checks that a binary is the proven output of
//! compiling a given source (or source hash) with the pinned TCC guest.
//!
//! Usage: verifier <receipt.bin> <binary> [source.c]
//!
//! Needs no compiler, no toolchain, no re-execution — just the receipt,
//! the IMAGE_ID constant, and the artifacts being checked.

use std::fs;
use std::time::Instant;

use methods::COMPILE_GUEST_ID;
use risc0_zkvm::Receipt;
use sha2::{Digest, Sha256};

fn hex(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02x}")).collect()
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let [receipt_path, binary_path, rest @ ..] = args.as_slice() else {
        eprintln!("usage: verifier <receipt.bin> <binary> [source.c]");
        std::process::exit(2);
    };

    let receipt: Receipt =
        bincode::deserialize(&fs::read(receipt_path).expect("read receipt")).expect("decode receipt");
    let binary = fs::read(binary_path).expect("read binary");

    let t0 = Instant::now();
    receipt.verify(COMPILE_GUEST_ID).expect("RECEIPT INVALID");
    let dt = t0.elapsed();

    let j = &receipt.journal.bytes;
    assert_eq!(j.len(), 84, "unexpected journal size");
    let (h_src, blob_sha1, h_out) = (&j[0..32], &j[32..52], &j[52..84]);

    let h_bin: [u8; 32] = Sha256::digest(&binary).into();
    assert_eq!(h_bin.as_slice(), h_out, "BINARY DOES NOT MATCH PROOF");

    println!("receipt            : VALID (cryptographically verified in {dt:.2?})");
    println!("pinned compiler    : image id {}", hex(bytemuck::cast_slice(&COMPILE_GUEST_ID)));
    println!("sha256(source)     : {}", hex(h_src));
    println!("git blob sha1      : {} (cf. `git hash-object <source>`)", hex(blob_sha1));
    println!("sha256(binary)     : {} == {binary_path} ✓", hex(h_out));

    if let [source_path] = rest {
        let src = fs::read(source_path).expect("read source");
        let h: [u8; 32] = Sha256::digest(&src).into();
        assert_eq!(h.as_slice(), h_src, "SOURCE DOES NOT MATCH PROOF");
        println!("source             : {source_path} matches proof ✓");
    }
    println!("=> this binary was compiled from this source by the pinned compiler.");
}
