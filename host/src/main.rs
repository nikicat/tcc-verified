//! Host: runs compilation jobs in the zkVM guest (TCC), producing compiled
//! artifacts, receipts (proofs), and timing/size measurements.
//!
//! Single file (one job: compile + link, legacy artifact names):
//!   host <source.c> [--mode composite|succinct|groth16] [--dev] [--out-dir DIR]
//!
//! Multi-TU build (N batched compile jobs + one link job, receipts chain by
//! object-file hashes — see jobfmt::verify_chain):
//!   host --build <build.json> [--mode ...] [--dev] [--out-dir DIR] [--only N|link]
//!
//! --only runs a single job of the build (for fanning batches out across
//! machines); the link run collects every receipt.job*.bin in the out dir
//! into chain.json.

use std::fs;
use std::os::unix::fs::PermissionsExt;
use std::path::{Path, PathBuf};
use std::time::{Duration, Instant};

use jobfmt::{hex, InputFile, Job, Manifest, Op};
use methods::{COMPILE_GUEST_ELF, COMPILE_GUEST_ID};
use risc0_zkvm::{default_prover, ExecutorEnv, InnerReceipt, ProverOpts, Receipt};
use serde::Deserialize;
use sha2::{Digest as _, Sha256};

#[derive(Deserialize)]
struct BuildSpec {
    /// Files shipped to every compile job (headers etc.). `path` is the
    /// in-guest path; `from` (default: same as `path`) is the disk path
    /// relative to the manifest's directory. Directories are walked
    /// recursively.
    #[serde(default)]
    files: Vec<FileSpec>,
    #[serde(default)]
    include_dirs: Vec<String>,
    #[serde(default)]
    defines: Vec<(String, Option<String>)>,
    /// Translation units; each becomes one compile op, batched
    /// `batch_size` per job/receipt.
    tus: Vec<TuSpec>,
    #[serde(default = "default_batch_size")]
    batch_size: usize,
    /// In-guest name of the linked executable (also the on-disk name).
    output: String,
}

fn default_batch_size() -> usize {
    16
}

#[derive(Deserialize)]
struct FileSpec {
    path: String,
    from: Option<String>,
}

#[derive(Deserialize)]
struct TuSpec {
    src: String,
    from: Option<String>,
    #[serde(default)]
    defines: Vec<(String, Option<String>)>,
}

const CHAIN_FORMAT: &str = "tcc-verified-chain/1";

#[derive(serde::Serialize)]
struct ChainFile {
    format: String,
    image_id: String,
    dev_mode: bool,
    output: String,
    receipts: Vec<String>,
}

fn obj_path(src: &str) -> String {
    let stem = src.strip_suffix(".c").or_else(|| src.strip_suffix(".s")).unwrap_or(src);
    format!("obj/{stem}.o")
}

/// Load a file or directory tree from disk into in-guest input files.
fn load_tree(disk: &Path, mem: &str, out: &mut Vec<InputFile>) {
    let meta = fs::metadata(disk).unwrap_or_else(|e| panic!("stat {}: {e}", disk.display()));
    if meta.is_dir() {
        let mut entries: Vec<_> = fs::read_dir(disk)
            .unwrap()
            .map(|e| e.unwrap().path())
            .collect();
        entries.sort();
        for p in entries {
            let name = p.file_name().unwrap().to_str().expect("non-utf8 filename");
            load_tree(&p, &format!("{mem}/{name}"), out);
        }
    } else {
        out.push(InputFile { path: mem.to_string(), data: fs::read(disk).unwrap() });
    }
}

struct JobResult {
    receipt: Receipt,
    outputs: Vec<(String, Vec<u8>)>,
    stats: (u64, u64, u64), // user cycles, total cycles, segments
    dt: Duration,
}

fn receipt_kind(receipt: &Receipt) -> &'static str {
    match &receipt.inner {
        InnerReceipt::Composite(_) => "composite",
        InnerReceipt::Succinct(_) => "succinct",
        InnerReceipt::Groth16(_) => "groth16",
        _ => "fake",
    }
}

fn run_job(job: &Job, opts: &ProverOpts, label: &str) -> JobResult {
    let input_bytes: usize = job.files.iter().map(|f| f.data.len()).sum();
    println!(
        "[{label}] proving: {} files ({input_bytes} bytes), {} ops...",
        job.files.len(),
        job.ops.len()
    );
    // framing: u32 byte length + the blob packed into u32 words (LE);
    // env::read_frame is unstable in risc0 3.0.5
    let bytes = job.encode();
    let mut words = vec![0u32; bytes.len().div_ceil(4)];
    bytemuck::cast_slice_mut::<u32, u8>(&mut words)[..bytes.len()].copy_from_slice(&bytes);
    let mut stdout_buf = Vec::new();
    let env = ExecutorEnv::builder()
        .write(&(bytes.len() as u32))
        .unwrap()
        .write_slice(&words)
        .stdout(&mut stdout_buf)
        .build()
        .unwrap();
    let t0 = Instant::now();
    let info = default_prover()
        .prove_with_opts(env, COMPILE_GUEST_ELF, opts)
        .unwrap_or_else(|e| panic!("[{label}] proving failed: {e}"));
    let dt = t0.elapsed();
    let outputs = jobfmt::decode_outputs(&stdout_buf).expect("decode guest outputs");
    println!(
        "[{label}] done: {} user / {} total cycles, {} segments, {dt:.2?}",
        info.stats.user_cycles, info.stats.total_cycles, info.stats.segments
    );
    JobResult {
        receipt: info.receipt,
        outputs,
        stats: (info.stats.user_cycles, info.stats.total_cycles, info.stats.segments as u64),
        dt,
    }
}

fn write_outputs(out_dir: &Path, outputs: &[(String, Vec<u8>)]) {
    for (path, data) in outputs {
        let disk = out_dir.join(path);
        fs::create_dir_all(disk.parent().unwrap()).unwrap();
        fs::write(&disk, data).unwrap();
    }
}

fn save_receipt(out_dir: &Path, name: &str, receipt: &Receipt) -> (String, usize) {
    let bytes = bincode::serialize(receipt).unwrap();
    let file = format!("receipt.{name}.{}.bin", receipt_kind(receipt));
    fs::write(out_dir.join(&file), &bytes).unwrap();
    (file, bytes.len())
}

/// Verify receipts + hash chain + exe bytes; returns the chain summary.
fn check_chain(receipts: &[Receipt], exe: &[u8]) -> jobfmt::ChainSummary {
    let mut manifests = Vec::new();
    for r in receipts {
        r.verify(COMPILE_GUEST_ID).expect("receipt verification failed");
        manifests.push(Manifest::decode(&r.journal.bytes).expect("decode journal"));
    }
    let summary = jobfmt::verify_chain(&manifests).expect("hash chain verification failed");
    let h: [u8; 32] = Sha256::digest(exe).into();
    assert_eq!(h, summary.executable.sha256, "exe bytes do not match proven hash");
    summary
}

fn main() {
    tracing_subscriber::fmt()
        .with_env_filter(tracing_subscriber::filter::EnvFilter::from_default_env())
        .init();

    let mut args = std::env::args().skip(1);
    let mut source = None;
    let mut build: Option<PathBuf> = None;
    let mut mode = "composite".to_string();
    let mut out_dir = PathBuf::from("out");
    let mut wrap: Option<PathBuf> = None;
    let mut only: Option<String> = None;
    while let Some(a) = args.next() {
        match a.as_str() {
            "--mode" => mode = args.next().expect("--mode needs a value"),
            "--dev" => std::env::set_var("RISC0_DEV_MODE", "1"),
            "--out-dir" => out_dir = args.next().expect("--out-dir needs a value").into(),
            "--build" => build = Some(args.next().expect("--build needs a manifest").into()),
            "--only" => only = Some(args.next().expect("--only needs N|link")),
            "--wrap" => wrap = Some(args.next().expect("--wrap needs a receipt path").into()),
            "--image-id" => {
                println!("{}", hex(bytemuck::cast_slice(&COMPILE_GUEST_ID)));
                return;
            }
            _ => source = Some(PathBuf::from(a)),
        }
    }

    // --wrap: compress an existing (e.g. remotely-proven succinct) receipt to
    // groth16 locally. Needs a docker-compatible CLI (podman: set DOCKER_HOST).
    if let Some(receipt_path) = wrap {
        let receipt: Receipt =
            bincode::deserialize(&fs::read(&receipt_path).expect("read receipt")).expect("decode");
        // pure compression: no IMAGE_ID check here, so receipts proven by the
        // canonical (docker-baked) guest wrap fine on a machine whose local
        // guest build differs. The verifier does the final ID check.
        fs::create_dir_all(&out_dir).expect("create out dir");
        println!("wrapping {} to groth16...", receipt_path.display());
        let t0 = Instant::now();
        let groth16 = default_prover()
            .compress(&ProverOpts::groth16(), &receipt)
            .unwrap();
        let dt = t0.elapsed();
        let bytes = bincode::serialize(&groth16).unwrap();
        let name = receipt_path
            .file_name()
            .unwrap()
            .to_string_lossy()
            .rsplit_once('.')
            .and_then(|(stem, _)| stem.rsplit_once('.'))
            .map(|(stem, _)| stem.to_string())
            .unwrap_or_else(|| "receipt".into());
        let path = out_dir.join(format!("{name}.groth16.bin"));
        fs::write(&path, &bytes).unwrap();
        println!("receipt            : {} ({} bytes, groth16)", path.display(), bytes.len());
        println!("wrap time          : {dt:.2?}");
        return;
    }

    let opts = match mode.as_str() {
        "composite" => ProverOpts::default(),
        "succinct" => ProverOpts::succinct(),
        "groth16" => ProverOpts::groth16(),
        m => panic!("unknown mode {m}"),
    };
    let dev_mode = std::env::var("RISC0_DEV_MODE").is_ok();
    fs::create_dir_all(&out_dir).expect("create out dir");
    println!("image id : {}", hex(bytemuck::cast_slice(&COMPILE_GUEST_ID)));

    if let Some(spec_path) = build {
        run_build(&spec_path, &opts, &out_dir, only.as_deref(), dev_mode);
        return;
    }

    // ---- single source file: one job = compile + link ----
    let source = source.expect(
        "usage: host <source.c> | --build <build.json> [--mode composite|succinct|groth16] [--dev] [--out-dir DIR]",
    );
    let src = fs::read(&source).expect("read source");
    let name = source.file_name().unwrap().to_string_lossy().to_string();
    let mem_src = format!("src/{name}");
    let obj = obj_path(&mem_src);
    let job = Job {
        files: vec![InputFile { path: mem_src.clone(), data: src }],
        ops: vec![
            Op::Compile {
                src: mem_src.clone(),
                out: obj.clone(),
                include_dirs: vec![],
                defines: vec![],
            },
            Op::Link { objs: vec![obj], out: "out.elf".into() },
        ],
    };

    println!("proving {} with mode={mode}...", source.display());
    let res = run_job(&job, &opts, "job");

    let summary = check_chain(std::slice::from_ref(&res.receipt), {
        let (_, elf) = res.outputs.iter().find(|(p, _)| p == "out.elf").expect("no out.elf");
        elf
    });

    let elf = &res.outputs.iter().find(|(p, _)| p == "out.elf").unwrap().1;
    let bin_path = out_dir.join("hello");
    fs::write(&bin_path, elf).unwrap();
    fs::set_permissions(&bin_path, fs::Permissions::from_mode(0o755)).unwrap();
    let (receipt_file, receipt_len) = {
        let bytes = bincode::serialize(&res.receipt).unwrap();
        let file = format!("receipt.{}.bin", receipt_kind(&res.receipt));
        fs::write(out_dir.join(&file), &bytes).unwrap();
        (file, bytes.len())
    };

    println!("--- compilation proved ---");
    println!("sha256(source)     : {}", hex(&summary.sources[0].sha256));
    println!("sha256(binary)     : {}", hex(&summary.executable.sha256));
    println!("binary             : {} ({} bytes)", bin_path.display(), elf.len());
    println!(
        "receipt            : {} ({receipt_len} bytes, {})",
        out_dir.join(receipt_file).display(),
        receipt_kind(&res.receipt)
    );
    println!(
        "guest cycles       : {} user / {} total, {} segments",
        res.stats.0, res.stats.1, res.stats.2
    );
    println!("proving time       : {:.2?}", res.dt);
    println!("verify (sanity)    : ok (receipt + chain + binary hash)");
}

fn run_build(spec_path: &Path, opts: &ProverOpts, out_dir: &Path, only: Option<&str>, dev_mode: bool) {
    let spec_dir = spec_path.parent().unwrap_or(Path::new(".")).to_path_buf();
    let spec: BuildSpec =
        serde_json::from_str(&fs::read_to_string(spec_path).expect("read build manifest"))
            .expect("parse build manifest");

    let mut shared: Vec<InputFile> = Vec::new();
    for f in &spec.files {
        load_tree(&spec_dir.join(f.from.as_ref().unwrap_or(&f.path)), &f.path, &mut shared);
    }
    let tus: Vec<(TuSpec, Vec<u8>)> = spec
        .tus
        .into_iter()
        .map(|t| {
            let data = fs::read(spec_dir.join(t.from.as_ref().unwrap_or(&t.src)))
                .unwrap_or_else(|e| panic!("read {}: {e}", t.src));
            (t, data)
        })
        .collect();

    let n_batches = tus.len().div_ceil(spec.batch_size);
    println!(
        "build: {} TUs in {n_batches} compile jobs (batch {}), {} shared files, link -> {}",
        tus.len(),
        spec.batch_size,
        shared.len(),
        spec.output
    );

    let total = Instant::now();
    let mut receipts: Vec<Receipt> = Vec::new();
    for (bi, batch) in tus.chunks(spec.batch_size).enumerate() {
        match only {
            Some("link") => continue,
            Some(n) if n.parse::<usize>().ok() != Some(bi) => continue,
            _ => {}
        }
        let mut files = shared.clone();
        let mut ops = Vec::new();
        for (tu, data) in batch {
            // a TU may also appear among the shared files (e.g. a source
            // that others #include); the guest treats duplicates as fatal
            match files.iter().find(|f| f.path == tu.src) {
                Some(f) if f.data == *data => {}
                Some(_) => panic!("conflicting contents for {}", tu.src),
                None => files.push(InputFile { path: tu.src.clone(), data: data.clone() }),
            }
            let mut defines = spec.defines.clone();
            defines.extend(tu.defines.iter().cloned());
            ops.push(Op::Compile {
                src: tu.src.clone(),
                out: obj_path(&tu.src),
                include_dirs: spec.include_dirs.clone(),
                defines,
            });
        }
        let res = run_job(&Job { files, ops }, opts, &format!("job{bi:03}"));
        write_outputs(out_dir, &res.outputs);
        save_receipt(out_dir, &format!("job{bi:03}"), &res.receipt);
        receipts.push(res.receipt);
    }

    if matches!(only, Some(n) if n != "link") {
        println!("done (--only {}): receipts + objects in {}", only.unwrap(), out_dir.display());
        return;
    }

    // ---- link job: inputs are the proven .o files from the out dir ----
    let objs: Vec<String> = tus.iter().map(|(t, _)| obj_path(&t.src)).collect();
    let files: Vec<InputFile> = objs
        .iter()
        .map(|o| InputFile {
            path: o.clone(),
            data: fs::read(out_dir.join(o))
                .unwrap_or_else(|e| panic!("read {o}: {e} (run the compile jobs first)")),
        })
        .collect();
    let link_job = Job {
        files,
        ops: vec![Op::Link { objs, out: spec.output.clone() }],
    };
    let res = run_job(&link_job, opts, "link");
    write_outputs(out_dir, &res.outputs);
    let exe_path = out_dir.join(&spec.output);
    fs::set_permissions(&exe_path, fs::Permissions::from_mode(0o755)).unwrap();
    save_receipt(out_dir, "link", &res.receipt);

    // collect the full chain (receipts may come from earlier --only runs)
    let mut receipt_files: Vec<String> = fs::read_dir(out_dir)
        .unwrap()
        .filter_map(|e| e.unwrap().file_name().into_string().ok())
        .filter(|n| n.starts_with("receipt.job") || n.starts_with("receipt.link"))
        .filter(|n| n.ends_with(".bin"))
        .collect();
    receipt_files.sort();
    let chain = ChainFile {
        format: CHAIN_FORMAT.into(),
        image_id: hex(bytemuck::cast_slice(&COMPILE_GUEST_ID)),
        dev_mode,
        output: spec.output.clone(),
        receipts: receipt_files.clone(),
    };
    fs::write(out_dir.join("chain.json"), serde_json::to_vec_pretty(&chain).unwrap()).unwrap();

    let all_receipts: Vec<Receipt> = receipt_files
        .iter()
        .map(|f| bincode::deserialize(&fs::read(out_dir.join(f)).unwrap()).expect("decode receipt"))
        .collect();
    let exe = fs::read(&exe_path).unwrap();
    let summary = check_chain(&all_receipts, &exe);

    println!("--- build proved ---");
    println!("executable         : {} ({} bytes)", exe_path.display(), exe.len());
    println!("sha256(executable) : {}", hex(&summary.executable.sha256));
    println!(
        "chain              : {} receipts, {} TUs, {} source files, {} objects",
        all_receipts.len(),
        summary.tu_count,
        summary.sources.len(),
        summary.objects.len()
    );
    println!("chain.json         : {}", out_dir.join("chain.json").display());
    println!("total time         : {:.2?}", total.elapsed());
    println!("verify (sanity)    : ok (receipts + chain + binary hash)");
}
