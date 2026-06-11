//! Standalone provenance verification. Three modes:
//!
//!   verifier --chain <chain.json> [--exe <path>] [--git-dir <repo>]...
//!       Verify a receipt chain (multi-TU build): every receipt against the
//!       pin, the hash chain (link inputs = compile outputs), and the
//!       executable bytes. With --git-dir (repeatable), additionally link
//!       proven source files to repo history by content.
//!
//!   verifier --exe <path> [--attestation <f.prov.json>] [--git-dir <repo>]...
//!       Verify an executable against its provenance attestation (read from
//!       --attestation, or from the trailer embedded in the exe).
//!
//!   verifier <receipt.bin> <binary> [source.c]          (legacy/raw mode)
//!
//! Trust root: the pinned canonical compiler id, embedded at build time from
//! CANONICAL_IMAGE_ID (override with --image-id). Everything inside an
//! attestation/chain file is attacker-controlled until the proofs verify
//! against that pin. Exit code 0 = verified.
//!
//! Git linkage is informational: it reports, per proven source file, which
//! repo path's content matches the proven sha256. Files not in any repo
//! (e.g. generated headers) are listed so the consumer can audit them.

mod attestation;

use std::collections::HashMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::time::Instant;

use attestation::{hex, split_trailer, Attestation, SourceClaim, FORMAT};
use base64::Engine as _;
use jobfmt::{ChainSummary, FileHash, Manifest};
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

/// Cryptographically verify receipts against the pin, then check the hash
/// chain. Returns the proven summary.
fn verify_receipts(receipts: &[Receipt], pin: [u32; 8]) -> (ChainSummary, std::time::Duration) {
    let t0 = Instant::now();
    let mut manifests = Vec::new();
    for (i, r) in receipts.iter().enumerate() {
        r.verify(pin)
            .unwrap_or_else(|e| fail(&format!("receipt {i}: proof invalid: {e}")));
        manifests.push(
            Manifest::decode(&r.journal.bytes)
                .unwrap_or_else(|e| fail(&format!("receipt {i}: bad journal: {e}"))),
        );
    }
    let summary = jobfmt::verify_chain(&manifests).unwrap_or_else(|e| fail(&e));
    (summary, t0.elapsed())
}

fn check_exe(exe: &[u8], summary: &ChainSummary) {
    let h: [u8; 32] = Sha256::digest(exe).into();
    if h != summary.executable.sha256 {
        fail("executable does not match the proven binary hash");
    }
}

/// One repo's `path -> blob sha1` map from HEAD's tree.
struct Repo {
    dir: String,
    head: String,
    tree: HashMap<String, String>,
}

fn load_repo(dir: &str) -> Repo {
    let out = |args: &[&str]| -> String {
        let o = Command::new("git").arg("-C").arg(dir).args(args).output().expect("run git");
        if !o.status.success() {
            fail(&format!("git {} failed in {dir}", args.join(" ")));
        }
        String::from_utf8_lossy(&o.stdout).into_owned()
    };
    let head = out(&["rev-parse", "HEAD"]).trim().to_string();
    let tree = out(&["ls-tree", "-r", "HEAD"])
        .lines()
        .filter_map(|l| {
            // <mode> blob <sha>\t<path>
            let (meta, path) = l.split_once('\t')?;
            let sha = meta.split_whitespace().nth(2)?;
            Some((path.to_string(), sha.to_string()))
        })
        .collect();
    Repo { dir: dir.to_string(), head, tree }
}

/// Find the proven file in the repo by path (exact, then suffix match) and
/// confirm the blob's *content* hashes to the proven sha256.
fn link_source(repos: &[Repo], f: &FileHash) -> Option<String> {
    for repo in repos {
        let candidates: Vec<&String> = match repo.tree.get(&f.path) {
            Some(_) => vec![&f.path],
            None => repo
                .tree
                .keys()
                .filter(|p| p.ends_with(&format!("/{}", f.path)))
                .collect(),
        };
        for path in candidates {
            let sha1 = &repo.tree[path];
            let content = Command::new("git")
                .args(["-C", &repo.dir, "cat-file", "blob", sha1])
                .output()
                .expect("run git");
            if !content.status.success() {
                continue;
            }
            let h: [u8; 32] = Sha256::digest(&content.stdout).into();
            if h == f.sha256 {
                return Some(format!("{}@{} {}", repo.dir, &repo.head[..12], path));
            }
        }
    }
    None
}

fn git_linkage(git_dirs: &[String], sources: &[FileHash]) {
    if git_dirs.is_empty() {
        return;
    }
    let repos: Vec<Repo> = git_dirs.iter().map(|d| load_repo(d)).collect();
    let mut linked = 0;
    let mut unlinked: Vec<&FileHash> = Vec::new();
    for f in sources {
        match link_source(&repos, f) {
            Some(loc) => {
                linked += 1;
                if sources.len() <= 8 {
                    println!("git linkage        : {} -> {loc} ✓", f.path);
                }
            }
            None => unlinked.push(f),
        }
    }
    if sources.len() > 8 {
        println!("git linkage        : {linked}/{} source files matched by content ✓", sources.len());
    }
    for f in &unlinked {
        println!("git linkage        : {} NOT in any given repo (sha256 {})", f.path, hex(&f.sha256));
    }
}

#[derive(serde::Deserialize)]
struct ChainFile {
    format: String,
    output: String,
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
    let image_id_arg = take_opt("--image-id");
    let chain_path = take_opt("--chain");
    let exe_path = take_opt("--exe");
    let attestation_path = take_opt("--attestation");
    let mut git_dirs: Vec<String> = Vec::new();
    while let Some(d) = take_opt("--git-dir") {
        git_dirs.push(d);
    }

    // trust root: explicit flag > pin embedded at build time
    let pin_hex = image_id_arg
        .or_else(|| Some(env!("CANONICAL_IMAGE_ID_HEX").to_string()).filter(|s| !s.is_empty()))
        .unwrap_or_else(|| {
            eprintln!("no pinned image id (built without CANONICAL_IMAGE_ID); pass --image-id");
            std::process::exit(2);
        });
    let pin = parse_image_id(&pin_hex);

    if let Some(chain_path) = chain_path {
        // ---- chain mode: multi-TU build ----
        let chain_path = PathBuf::from(chain_path);
        let dir = chain_path.parent().unwrap_or(Path::new("."));
        let chain: ChainFile =
            serde_json::from_slice(&fs::read(&chain_path).expect("read chain.json"))
                .expect("parse chain.json");
        if chain.format != "tcc-verified-chain/1" {
            fail(&format!("unsupported chain format {}", chain.format));
        }
        let receipts: Vec<Receipt> = chain
            .receipts
            .iter()
            .map(|f| {
                bincode::deserialize(&fs::read(dir.join(f)).expect("read receipt"))
                    .expect("decode receipt")
            })
            .collect();
        let (summary, dt) = verify_receipts(&receipts, pin);
        let exe_file = exe_path.map(PathBuf::from).unwrap_or_else(|| dir.join(&chain.output));
        let exe = fs::read(&exe_file).expect("read executable");
        let (exe_bytes, _) = split_trailer(&exe);
        check_exe(exe_bytes, &summary);

        println!("proofs             : {} receipts VALID ({dt:.2?})", receipts.len());
        println!("pinned compiler    : {pin_hex}");
        println!(
            "chain              : {} TUs, {} source files, {} objects",
            summary.tu_count,
            summary.sources.len(),
            summary.objects.len()
        );
        println!(
            "binary sha256      : {} == {} ✓",
            hex(&summary.executable.sha256),
            exe_file.display()
        );
        git_linkage(&git_dirs, &summary.sources);
        println!("=> this executable was compiled from these sources by the pinned compiler.");
        return;
    }

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
        let receipts: Vec<Receipt> = att
            .proof
            .receipts_bincode_b64
            .iter()
            .map(|b| {
                bincode::deserialize(
                    &base64::engine::general_purpose::STANDARD.decode(b).expect("decode b64"),
                )
                .expect("decode receipt")
            })
            .collect();

        let (summary, dt) = verify_receipts(&receipts, pin);
        check_exe(exe, &summary);

        // cleartext claims must match the proven chain (so they can't lie)
        let proven: Vec<SourceClaim> = summary
            .sources
            .iter()
            .map(|f| SourceClaim { path: f.path.clone(), sha256: hex(&f.sha256) })
            .collect();
        if att.claims.binary_sha256 != hex(&summary.executable.sha256)
            || att.claims.sources != proven
        {
            fail("attestation cleartext claims do not match the proven chain");
        }

        println!("proofs             : {} receipts VALID ({dt:.2?})", receipts.len());
        println!("pinned compiler    : {pin_hex}");
        println!("                     {}", att.compiler.description);
        println!(
            "claims             : {} TUs, {} source files",
            summary.tu_count,
            summary.sources.len()
        );
        println!("binary sha256      : {} == {exe_path} ✓", hex(&summary.executable.sha256));
        git_linkage(&git_dirs, &summary.sources);
        println!("=> this executable was compiled from these sources by the pinned compiler.");
        return;
    }

    // ---- legacy/raw mode: verifier <receipt.bin> <binary> [source.c] ----
    let [receipt_path, binary_path, rest @ ..] = args.as_slice() else {
        eprintln!("usage: verifier --chain <chain.json> [--exe <path>] [--git-dir <repo>]... [--image-id <hex>]");
        eprintln!("       verifier --exe <path> [--attestation <f>] [--git-dir <repo>]... [--image-id <hex>]");
        eprintln!("       verifier <receipt.bin> <binary> [source.c] [--image-id <hex>]");
        std::process::exit(2);
    };
    let _ = COMPILE_GUEST_ID; // local guest id intentionally unused: pin is canonical

    let receipt: Receipt =
        bincode::deserialize(&fs::read(receipt_path).expect("read receipt")).expect("decode receipt");
    let binary = fs::read(binary_path).expect("read binary");

    let (summary, dt) = verify_receipts(std::slice::from_ref(&receipt), pin);
    let (binary_bytes, _) = split_trailer(&binary);
    check_exe(binary_bytes, &summary);

    println!("receipt            : VALID (cryptographically verified in {dt:.2?})");
    println!("pinned compiler    : image id {pin_hex}");
    for s in &summary.sources {
        println!("source             : {} sha256 {}", s.path, hex(&s.sha256));
    }
    println!("sha256(binary)     : {} == {binary_path} ✓", hex(&summary.executable.sha256));
    if let [source_path] = rest {
        let src = fs::read(source_path).expect("read source");
        let h: [u8; 32] = Sha256::digest(&src).into();
        if !summary.sources.iter().any(|s| s.sha256 == h) {
            fail("source does not match any proven input hash");
        }
        println!("source             : {source_path} matches proof ✓");
    }
    println!("=> this binary was compiled from these sources by the pinned compiler.");
}
