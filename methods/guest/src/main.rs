//! zkVM guest: compile a C source with the embedded TCC and commit
//! (sha256(source), git-blob-sha1(source), sha256(elf)) to the journal.
//!
//! The TCC binary, its flags and the syscall shims are baked into this image,
//! so the IMAGE_ID pins the entire compiler — the journal only needs to bind
//! the source to the output.

use risc0_zkvm::guest::env;
use risc0_zkvm::sha::{Impl, Sha256};
use sha1::{Digest as _, Sha1};

extern "C" {
    fn zk_compile(
        src: *const u8,
        out: *mut *const u8,
        out_len: *mut u32,
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

fn main() {
    let src: Vec<u8> = env::read();

    // git blob hash: sha1("blob <len>\0" || content) — links to a commit via
    // the commit->tree->blob merkle chain, which is verifiable outside the proof.
    let mut h = Sha1::new();
    h.update(format!("blob {}\0", src.len()).as_bytes());
    h.update(&src);
    let blob_sha1: [u8; 20] = h.finalize().into();

    let h_src = *Impl::hash_bytes(&src);

    let mut csrc = src; // NUL-terminate for tcc_compile_string
    csrc.push(0);

    let (mut out, mut out_len) = (core::ptr::null(), 0u32);
    let (mut err, mut err_len) = (core::ptr::null(), 0u32);
    let rc = unsafe {
        zk_compile(csrc.as_ptr(), &mut out, &mut out_len, &mut err, &mut err_len)
    };
    if rc != 0 {
        let msg = if err.is_null() {
            "".into()
        } else {
            String::from_utf8_lossy(unsafe {
                core::slice::from_raw_parts(err, err_len as usize)
            })
        };
        panic!("tcc failed (rc={rc}):\n{msg}");
    }
    let elf = unsafe { core::slice::from_raw_parts(out, out_len as usize) };
    let h_out = *Impl::hash_bytes(elf);

    // journal: 32 + 20 + 32 = 84 bytes
    let mut journal = Vec::with_capacity(84);
    journal.extend_from_slice(h_src.as_bytes());
    journal.extend_from_slice(&blob_sha1);
    journal.extend_from_slice(h_out.as_bytes());
    env::commit_slice(&journal);

    // the binary itself goes to the host unproven; only its hash matters
    env::write_slice(elf);
}
