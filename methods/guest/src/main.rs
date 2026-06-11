//! zkVM guest: run a compilation job (compile TUs to objects and/or link
//! objects into an executable) with the embedded TCC, and commit the job
//! manifest — ops + sha256 of every input and output file — to the journal.
//!
//! The TCC binary, its flags and the syscall shims are baked into this image,
//! so the IMAGE_ID pins the entire compiler — the journal only needs to bind
//! the inputs to the outputs.

use std::ffi::CString;
use std::os::raw::c_char;

use jobfmt::{FileHash, Job, Manifest, Op};
use risc0_zkvm::guest::env;
use risc0_zkvm::sha::{Impl, Sha256};

extern "C" {
    fn zk_add_file(path: *const c_char, data: *const u8, len: u32);
    fn zk_get_file(path: *const c_char, data: *mut *const u8, len: *mut u32) -> i32;
    fn zk_compile_obj(
        src: *const c_char,
        out: *const c_char,
        inc_dirs: *const *const c_char,
        n_inc: u32,
        def_names: *const *const c_char,
        def_values: *const *const c_char,
        n_def: u32,
        err: *mut *const u8,
        err_len: *mut u32,
    ) -> i32;
    fn zk_link(
        objs: *const *const c_char,
        n_objs: u32,
        out: *const c_char,
        err: *mut *const u8,
        err_len: *mut u32,
    ) -> i32;
    fn zk_console(p: *mut *const u8, n: *mut u32);
}

/// Called by the C shims on _exit/_kill (e.g. assertion failures inside TCC).
#[no_mangle]
extern "C" fn zk_rust_abort(code: i32) -> ! {
    let (mut p, mut n) = (core::ptr::null(), 0u32);
    unsafe { zk_console(&mut p, &mut n) };
    let con = if p.is_null() {
        "".into()
    } else {
        String::from_utf8_lossy(unsafe { core::slice::from_raw_parts(p, n as usize) })
    };
    panic!("C runtime _exit({code}); console:\n{con}");
}

fn cstr(s: &str) -> CString {
    CString::new(s).expect("NUL in path")
}

fn sha256(data: &[u8]) -> [u8; 32] {
    let mut h = [0u8; 32];
    h.copy_from_slice(Impl::hash_bytes(data).as_bytes());
    h
}

fn get_file(path: &str) -> &'static [u8] {
    let cpath = cstr(path);
    let (mut p, mut n) = (core::ptr::null(), 0u32);
    let rc = unsafe { zk_get_file(cpath.as_ptr(), &mut p, &mut n) };
    if rc != 0 || p.is_null() {
        panic!("output file {path} was not produced");
    }
    unsafe { core::slice::from_raw_parts(p, n as usize) }
}

/// Input framing (env::read_frame is unstable in risc0 3.0.5): a u32 byte
/// length, then the job blob packed into u32 words (LE).
fn read_input() -> Vec<u8> {
    let n: u32 = env::read();
    let mut words = vec![0u32; (n as usize).div_ceil(4)];
    env::read_slice(&mut words);
    let bytes =
        unsafe { core::slice::from_raw_parts(words.as_ptr() as *const u8, n as usize) };
    bytes.to_vec()
}

fn main() {
    let job = Job::decode(&read_input()).expect("decode job");

    // hash + register inputs (data stays owned by `job` for the whole run)
    let mut inputs = Vec::with_capacity(job.files.len());
    for f in &job.files {
        inputs.push(FileHash { path: f.path.clone(), sha256: sha256(&f.data) });
        let cpath = cstr(&f.path);
        unsafe { zk_add_file(cpath.as_ptr(), f.data.as_ptr(), f.data.len() as u32) };
    }

    for (i, op) in job.ops.iter().enumerate() {
        let (mut err, mut err_len) = (core::ptr::null(), 0u32);
        let rc = match op {
            Op::Compile { src, out, include_dirs, defines } => {
                let csrc = cstr(src);
                let cout = cstr(out);
                let cinc: Vec<CString> = include_dirs.iter().map(|d| cstr(d)).collect();
                let pinc: Vec<*const c_char> = cinc.iter().map(|c| c.as_ptr()).collect();
                let cnames: Vec<CString> = defines.iter().map(|(n, _)| cstr(n)).collect();
                let cvals: Vec<Option<CString>> =
                    defines.iter().map(|(_, v)| v.as_deref().map(cstr)).collect();
                let pnames: Vec<*const c_char> = cnames.iter().map(|c| c.as_ptr()).collect();
                let pvals: Vec<*const c_char> = cvals
                    .iter()
                    .map(|v| v.as_ref().map_or(core::ptr::null(), |c| c.as_ptr()))
                    .collect();
                unsafe {
                    zk_compile_obj(
                        csrc.as_ptr(),
                        cout.as_ptr(),
                        pinc.as_ptr(),
                        pinc.len() as u32,
                        pnames.as_ptr(),
                        pvals.as_ptr(),
                        pnames.len() as u32,
                        &mut err,
                        &mut err_len,
                    )
                }
            }
            Op::Link { objs, out } => {
                let cobjs: Vec<CString> = objs.iter().map(|o| cstr(o)).collect();
                let pobjs: Vec<*const c_char> = cobjs.iter().map(|c| c.as_ptr()).collect();
                let cout = cstr(out);
                unsafe {
                    zk_link(pobjs.as_ptr(), pobjs.len() as u32, cout.as_ptr(), &mut err, &mut err_len)
                }
            }
        };
        if rc != 0 {
            let msg = if err.is_null() {
                "".into()
            } else {
                String::from_utf8_lossy(unsafe {
                    core::slice::from_raw_parts(err, err_len as usize)
                })
            };
            panic!("op {i} ({op:?}) failed (rc={rc}):\n{msg}");
        }
    }

    // collect, hash and ship outputs
    let mut outputs = Vec::with_capacity(job.ops.len());
    let mut out_files = Vec::with_capacity(job.ops.len());
    for op in &job.ops {
        let path = op.out_path();
        let data = get_file(path);
        outputs.push(FileHash { path: path.into(), sha256: sha256(data) });
        out_files.push((path.to_string(), data.to_vec()));
    }

    let manifest = Manifest { ops: job.ops.clone(), inputs, outputs };
    env::commit_slice(&manifest.encode());

    // the output bytes themselves go to the host unproven; only hashes matter
    env::write_slice(&jobfmt::encode_outputs(&out_files));
}
