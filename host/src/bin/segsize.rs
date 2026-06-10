//! Execution-only: run the TCC guest and report per-segment serialized sizes,
//! to size up shipping segments to remote provers.
//!
//! Usage: segsize <source.c>

use methods::COMPILE_GUEST_ELF;
use risc0_zkvm::{ExecutorEnv, ExecutorImpl};
use std::time::Instant;

fn main() {
    let source = std::env::args().nth(1).expect("usage: segsize <source.c>");
    let src = std::fs::read(&source).expect("read source");

    let mut compiled: Vec<u8> = Vec::new();
    let env = ExecutorEnv::builder()
        .write(&src)
        .unwrap()
        .stdout(&mut compiled)
        .build()
        .unwrap();

    let t0 = Instant::now();
    let mut exec = ExecutorImpl::from_elf(env, COMPILE_GUEST_ELF).unwrap();
    let session = exec.run().unwrap();
    let dt = t0.elapsed();

    println!("source: {source} ({} bytes)", src.len());
    println!("execution (no proving): {dt:.2?}, {} user cycles, {} segments",
        session.user_cycles, session.segments.len());

    let mut total = 0usize;
    for (i, seg_ref) in session.segments.iter().enumerate() {
        let seg = seg_ref.resolve().unwrap();
        let bytes = bincode::serialize(&seg).unwrap();
        total += bytes.len();
        println!("  segment {i}: po2={} serialized {} bytes", seg.po2(), bytes.len());
    }
    println!("total serialized segments: {} bytes ({:.1} MB)", total, total as f64 / 1e6);
}
