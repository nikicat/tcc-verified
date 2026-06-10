//! Host: runs the TCC guest in the zkVM, producing the compiled binary,
//! the receipt (proof), and timing/size measurements.
//!
//! Usage: host <source.c> [--mode composite|succinct|groth16] [--dev] [--out-dir DIR]

use std::fs;
use std::os::unix::fs::PermissionsExt;
use std::path::PathBuf;
use std::time::Instant;

use methods::{COMPILE_GUEST_ELF, COMPILE_GUEST_ID};
use risc0_zkvm::{default_prover, ExecutorEnv, InnerReceipt, ProverOpts};

fn hex(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02x}")).collect()
}

fn main() {
    tracing_subscriber::fmt()
        .with_env_filter(tracing_subscriber::filter::EnvFilter::from_default_env())
        .init();

    let mut args = std::env::args().skip(1);
    let mut source = None;
    let mut mode = "composite".to_string();
    let mut out_dir = PathBuf::from("out");
    while let Some(a) = args.next() {
        match a.as_str() {
            "--mode" => mode = args.next().expect("--mode needs a value"),
            "--dev" => std::env::set_var("RISC0_DEV_MODE", "1"),
            "--out-dir" => out_dir = args.next().expect("--out-dir needs a value").into(),
            _ => source = Some(PathBuf::from(a)),
        }
    }
    let source = source.expect("usage: host <source.c> [--mode composite|succinct|groth16] [--dev] [--out-dir DIR]");
    let src = fs::read(&source).expect("read source");
    fs::create_dir_all(&out_dir).expect("create out dir");

    let mut compiled: Vec<u8> = Vec::new();
    let env = ExecutorEnv::builder()
        .write(&src)
        .unwrap()
        .stdout(&mut compiled)
        .build()
        .unwrap();

    let opts = match mode.as_str() {
        "composite" => ProverOpts::default(),
        "succinct" => ProverOpts::succinct(),
        "groth16" => ProverOpts::groth16(),
        m => panic!("unknown mode {m}"),
    };

    println!("image id : {}", hex(bytemuck::cast_slice(&COMPILE_GUEST_ID)));
    println!("proving {} ({} bytes) with mode={mode}...", source.display(), src.len());
    let t0 = Instant::now();
    let info = default_prover()
        .prove_with_opts(env, COMPILE_GUEST_ELF, &opts)
        .unwrap();
    let dt = t0.elapsed();

    let receipt = info.receipt;
    let journal = receipt.journal.bytes.clone();
    assert_eq!(journal.len(), 84, "unexpected journal size");

    let kind = match &receipt.inner {
        InnerReceipt::Composite(_) => "composite",
        InnerReceipt::Succinct(_) => "succinct",
        InnerReceipt::Groth16(_) => "groth16",
        _ => "fake (dev mode)",
    };
    let receipt_bytes = bincode::serialize(&receipt).unwrap();

    let bin_path = out_dir.join("hello");
    fs::write(&bin_path, &compiled).unwrap();
    fs::set_permissions(&bin_path, fs::Permissions::from_mode(0o755)).unwrap();
    let receipt_path = out_dir.join(format!("receipt.{kind}.bin"));
    fs::write(&receipt_path, &receipt_bytes).unwrap();

    println!("--- compilation proved ---");
    println!("sha256(source)     : {}", hex(&journal[0..32]));
    println!("git blob sha1      : {}", hex(&journal[32..52]));
    println!("sha256(binary)     : {}", hex(&journal[52..84]));
    println!("binary             : {} ({} bytes)", bin_path.display(), compiled.len());
    println!("receipt            : {} ({} bytes, {kind})", receipt_path.display(), receipt_bytes.len());
    println!("guest cycles       : {} user / {} total, {} segments",
        info.stats.user_cycles, info.stats.total_cycles, info.stats.segments);
    println!("proving time       : {dt:.2?}");

    let t1 = Instant::now();
    receipt.verify(COMPILE_GUEST_ID).expect("receipt verification failed");
    println!("verify (sanity)    : ok in {:.2?}", t1.elapsed());
}
