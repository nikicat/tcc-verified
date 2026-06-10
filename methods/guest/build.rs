//! Cross-compiles vendored TCC + newlib syscall shims to riscv32im and links
//! them into the guest. The exact TCC sources, defines and flags here are all
//! pinned by the guest IMAGE_ID — they are part of the proof statement.

use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

fn find_cpp_toolchain() -> PathBuf {
    if let Ok(p) = env::var("RISC0_CPP_TOOLCHAIN") {
        return PathBuf::from(p);
    }
    let home = env::var("HOME").expect("HOME not set");
    let toolchains = Path::new(&home).join(".risc0/toolchains");
    let dir = fs::read_dir(&toolchains)
        .expect("~/.risc0/toolchains missing — run `rzup install cpp`")
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .find(|p| p.file_name().unwrap().to_string_lossy().contains("-cpp-"))
        .expect("no cpp toolchain — run `rzup install cpp`");
    // layout: <ver>-cpp-<host>/riscv32im-*/bin/riscv32-unknown-elf-gcc
    fs::read_dir(&dir)
        .unwrap()
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .find(|p| p.file_name().unwrap().to_string_lossy().starts_with("riscv32im"))
        .expect("no riscv32im dir in cpp toolchain")
}

fn run(cmd: &mut Command) {
    let st = cmd.status().unwrap_or_else(|e| panic!("spawn {cmd:?}: {e}"));
    assert!(st.success(), "command failed: {cmd:?}");
}

fn main() {
    let out = PathBuf::from(env::var("OUT_DIR").unwrap());
    let manifest = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let vendor = manifest.join("../../vendor/tinycc").canonicalize().unwrap();
    let shim = manifest.join("shim");
    let tc = find_cpp_toolchain();
    let gcc = tc.join("bin/riscv32-unknown-elf-gcc");
    let ar = tc.join("bin/riscv32-unknown-elf-gcc-ar");

    let gen = out.join("gen");
    fs::create_dir_all(&gen).unwrap();

    // 1. tccdefs_.h: TCC's built-in preprocessor defs as C strings (so the
    //    compiler needs no include files at runtime). Same as TCC's Makefile.
    let c2str = out.join("c2str");
    run(Command::new("cc")
        .arg(vendor.join("conftest.c"))
        .arg("-DC2STR")
        .arg("-o")
        .arg(&c2str));
    run(Command::new(&c2str)
        .arg(vendor.join("include/tccdefs.h"))
        .arg(gen.join("tccdefs_.h")));

    // 2. minimal config.h (normally produced by ./configure)
    fs::write(
        gen.join("config.h"),
        "#define TCC_VERSION \"0.9.28rc\"\n\
         #define CONFIG_TCC_PREDEFS 1\n\
         #define CONFIG_TCCDIR \"/lib/tcc\"\n",
    )
    .unwrap();

    let cflags = [
        "-march=rv32im",
        "-mabi=ilp32",
        "-O2",
        "-w",
        "-DONE_SOURCE=1",
        "-DTCC_TARGET_X86_64=1",
        "-DCONFIG_TCC_STATIC=1",
        "-DCONFIG_TCC_SEMLOCK=0",
    ];

    // 3. libtcc.a (single translation unit pulls in tccpp/tccgen/tccelf/x86_64-gen/...)
    let libtcc_o = out.join("libtcc.o");
    run(Command::new(&gcc)
        .args(cflags)
        .arg("-I")
        .arg(&gen)
        .arg("-I")
        .arg(&vendor)
        .arg("-c")
        .arg(vendor.join("libtcc.c"))
        .arg("-o")
        .arg(&libtcc_o));
    run(Command::new(&ar).arg("rcs").arg(out.join("libtcc.a")).arg(&libtcc_o));

    // 4. libzkshim.a (newlib OS layer + zk_compile entry point)
    let mut shim_objs = Vec::new();
    for src in ["sys.c", "compile.c"] {
        let obj = out.join(format!("{src}.o"));
        run(Command::new(&gcc)
            .args(cflags)
            .arg("-I")
            .arg(&vendor) // libtcc.h
            .arg("-c")
            .arg(shim.join(src))
            .arg("-o")
            .arg(&obj));
        shim_objs.push(obj);
    }
    let mut ar_cmd = Command::new(&ar);
    ar_cmd.arg("rcs").arg(out.join("libzkshim.a"));
    ar_cmd.args(&shim_objs);
    run(&mut ar_cmd);

    // 5. newlib libc with the members that collide with the always-loaded
    //    Rust rlibs (compiler-builtins mem*, risc0 platform's libm exports)
    //    removed; those references resolve to the Rust-side definitions.
    let libc_filtered = out.join("libzknewlibc.a");
    fs::copy(tc.join("riscv32-unknown-elf/lib/libc.a"), &libc_filtered).unwrap();
    let conflicts = [
        "memcpy", "memcpy-asm", "memmove", "memmove-stub", "memset", "memcmp",
        "s_copysign", "sf_copysign", "s_frexp", "sf_frexp", "s_ldexp",
        "sf_ldexp", "s_modf", "sf_modf", "s_scalbn", "sf_scalbn",
    ];
    let members = Command::new(&ar).arg("t").arg(&libc_filtered).output().unwrap();
    for member in String::from_utf8(members.stdout).unwrap().lines() {
        let stem = member
            .trim_start_matches("libc_a-")
            .trim_start_matches("libm_a-")
            .trim_end_matches(".o");
        if conflicts.contains(&stem) {
            run(Command::new(&ar).arg("d").arg(&libc_filtered).arg(member));
        }
    }

    // 6. link: shims + tcc + filtered newlib + libgcc (soft-float, 64-bit arith)
    let libgcc = Command::new(&gcc)
        .arg("--print-libgcc-file-name")
        .output()
        .unwrap();
    let libgcc = PathBuf::from(String::from_utf8(libgcc.stdout).unwrap().trim());

    println!("cargo:rustc-link-search=native={}", out.display());
    println!("cargo:rustc-link-search=native={}", libgcc.parent().unwrap().display());
    println!("cargo:rustc-link-lib=static=zkshim");
    println!("cargo:rustc-link-lib=static=tcc");
    println!("cargo:rustc-link-lib=static=zknewlibc");
    println!("cargo:rustc-link-lib=static=gcc");

    println!("cargo:rerun-if-changed=shim");
    println!("cargo:rerun-if-changed={}", vendor.display());
    println!("cargo:rerun-if-env-changed=RISC0_CPP_TOOLCHAIN");
}
