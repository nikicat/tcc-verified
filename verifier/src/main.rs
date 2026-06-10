//! Standalone provenance verification. Two modes:
//!
//!   verifier --exe <path> [--attestation <f.prov.json>] [--git-dir <repo>]
//!       Verify an executable against its provenance attestation (read from
//!       --attestation, or from the trailer embedded in the exe). With
//!       --git-dir, additionally link the proven source blob to the repo's
//!       history (commit -> tree -> blob is git's own merkle chain).
//!
//!   verifier <receipt.bin> <binary> [source.c]          (legacy/raw mode)
//!
//! Trust root: the pinned canonical compiler id, embedded at build time from
//! CANONICAL_IMAGE_ID (override with --image-id). Everything inside an
//! attestation file is attacker-controlled until the proof verifies against
//! that pin. Exit code 0 = verified.

mod attestation;

use std::fs;
use std::process::Command;
use std::time::Instant;

use attestation::{hex, split_trailer, Attestation, FORMAT};
use base64::Engine as _;
use methods::COMPILE_GUEST_ID;
use risc0_zkvm::Receipt;
use sha2::{Digest, Sha256};

fn parse_image_id(hex_str: &str) -> [u32; 8] {
    let s = hex_str.trim();
    assert_eq!(s.len(), 64, "image id must be 64 hex chars");
    let mut bytes = [0u8; 32];
    for (i, b) in bytes.iter_mut().enumerate() {
        *b = u8::from_str_radix(&s[2 * i..2 * i + 2], 16).expect("bad hex in image id");
    }
    bytemuck::cast(bytes)
}

fn fail(msg: &str) -> ! {
    eprintln!("VERIFICATION FAILED: {msg}");
    std::process::exit(1);
}

/// journal layout: sha256(src) || git-blob-sha1(src) || sha256(elf)
fn split_journal(j: &[u8]) -> (&[u8], &[u8], &[u8]) {
    if j.len() != 84 {
        fail("unexpected journal size");
    }
    (&j[0..32], &j[32..52], &j[52..84])
}

fn git_linkage(git_dir: &str, blob_sha1: &str) {
    let typ = Command::new("git")
        .args(["-C", git_dir, "cat-file", "-t", blob_sha1])
        .output()
        .expect("run git");
    if !typ.status.success() || String::from_utf8_lossy(&typ.stdout).trim() != "blob" {
        fail(&format!("proven source blob {blob_sha1} not found in {git_dir}"));
    }
    let head = Command::new("git")
        .args(["-C", git_dir, "rev-parse", "HEAD"])
        .output()
        .expect("run git");
    let head = String::from_utf8_lossy(&head.stdout).trim().to_string();
    let tree = Command::new("git")
        .args(["-C", git_dir, "ls-tree", "-r", "HEAD"])
        .output()
        .expect("run git");
    let paths: Vec<String> = String::from_utf8_lossy(&tree.stdout)
        .lines()
        .filter(|l| l.contains(blob_sha1))
        .filter_map(|l| l.split('\t').nth(1).map(str::to_string))
        .collect();
    if paths.is_empty() {
        println!("git linkage        : blob {blob_sha1} exists in object store (not in HEAD tree)");
    } else {
        println!("git linkage        : commit {head} -> {} ✓", paths.join(", "));
    }
}

fn main() {
    let mut args: Vec<String> = std::env::args().skip(1).collect();

    let mut take_opt = |name: &str| -> Option<String> {
        args.iter().position(|a| a == name).map(|i| {
            args.remove(i);
            args.remove(i)
        })
    };
    let image_id_arg = take_opt("--image-id");
    let exe_path = take_opt("--exe");
    let attestation_path = take_opt("--attestation");
    let git_dir = take_opt("--git-dir");

    // trust root: explicit flag > pin embedded at build time
    let pin_hex = image_id_arg
        .or_else(|| Some(env!("CANONICAL_IMAGE_ID_HEX").to_string()).filter(|s| !s.is_empty()))
        .unwrap_or_else(|| {
            eprintln!("no pinned image id (built without CANONICAL_IMAGE_ID); pass --image-id");
            std::process::exit(2);
        });
    let pin = parse_image_id(&pin_hex);

    if let Some(exe_path) = exe_path {
        // ---- attestation mode ----
        let file = fs::read(&exe_path).expect("read exe");
        let (exe, trailer) = split_trailer(&file);
        let att_bytes = match (&attestation_path, trailer) {
            (Some(p), _) => fs::read(p).expect("read attestation"),
            (None, Some(t)) => t.to_vec(),
            (None, None) => fail("no --attestation given and no trailer embedded in exe"),
        };
        let att: Attestation = serde_json::from_slice(&att_bytes).expect("parse attestation json");
        if att.format != FORMAT {
            fail(&format!("unsupported attestation format {}", att.format));
        }
        if att.compiler.image_id != pin_hex {
            fail(&format!(
                "attestation is for compiler {} but your pin is {pin_hex}",
                att.compiler.image_id
            ));
        }
        let receipt: Receipt = bincode::deserialize(
            &base64::engine::general_purpose::STANDARD
                .decode(&att.proof.receipt_bincode_b64)
                .expect("decode receipt b64"),
        )
        .expect("decode receipt");

        let t0 = Instant::now();
        receipt.verify(pin).unwrap_or_else(|e| fail(&format!("proof invalid: {e}")));
        let dt = t0.elapsed();

        let (h_src, blob, h_out) = split_journal(&receipt.journal.bytes);
        // cleartext claims must match the proven journal (so they can't lie)
        if att.claims.source_sha256 != hex(h_src)
            || att.claims.source_git_blob_sha1 != hex(blob)
            || att.claims.binary_sha256 != hex(h_out)
        {
            fail("attestation cleartext claims do not match the proven journal");
        }
        let h_exe: [u8; 32] = Sha256::digest(exe).into();
        if h_exe.as_slice() != h_out {
            fail("executable does not match the proven binary hash");
        }

        println!("proof              : VALID ({dt:.2?})");
        println!("pinned compiler    : {pin_hex}");
        println!("                     {}", att.compiler.description);
        println!("source sha256      : {}", hex(h_src));
        println!("source git blob    : {}", hex(blob));
        println!("binary sha256      : {} == {exe_path} ✓", hex(h_out));
        if let Some(dir) = git_dir {
            git_linkage(&dir, &hex(blob));
        }
        println!("=> this executable was compiled from this source by the pinned compiler.");
        return;
    }

    // ---- legacy/raw mode: verifier <receipt.bin> <binary> [source.c] ----
    let [receipt_path, binary_path, rest @ ..] = args.as_slice() else {
        eprintln!("usage: verifier --exe <path> [--attestation <f>] [--git-dir <repo>] [--image-id <hex>]");
        eprintln!("       verifier <receipt.bin> <binary> [source.c] [--image-id <hex>]");
        std::process::exit(2);
    };
    let _ = COMPILE_GUEST_ID; // local guest id intentionally unused: pin is canonical

    let receipt: Receipt =
        bincode::deserialize(&fs::read(receipt_path).expect("read receipt")).expect("decode receipt");
    let binary = fs::read(binary_path).expect("read binary");

    let t0 = Instant::now();
    receipt.verify(pin).unwrap_or_else(|e| fail(&format!("proof invalid: {e}")));
    let dt = t0.elapsed();

    let (h_src, blob, h_out) = split_journal(&receipt.journal.bytes);
    let h_bin: [u8; 32] = Sha256::digest(&binary).into();
    if h_bin.as_slice() != h_out {
        fail("binary does not match the proven hash");
    }

    println!("receipt            : VALID (cryptographically verified in {dt:.2?})");
    println!("pinned compiler    : image id {pin_hex}");
    println!("sha256(source)     : {}", hex(h_src));
    println!("git blob sha1      : {} (cf. `git hash-object <source>`)", hex(blob));
    println!("sha256(binary)     : {} == {binary_path} ✓", hex(h_out));
    if let [source_path] = rest {
        let src = fs::read(source_path).expect("read source");
        let h: [u8; 32] = Sha256::digest(&src).into();
        if h.as_slice() != h_src {
            fail("source does not match the proven hash");
        }
        println!("source             : {source_path} matches proof ✓");
    }
    println!("=> this binary was compiled from this source by the pinned compiler.");
}
