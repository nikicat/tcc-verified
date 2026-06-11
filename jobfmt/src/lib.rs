//! Shared job/journal format between the zkVM guest, the host driver and the
//! verifier.
//!
//! A *job* is one guest execution = one receipt: a file tree plus a list of
//! ops (compile a TU to an object file, or link object files into an
//! executable). The guest commits a *manifest* to the journal: the ops it ran,
//! the sha256 of every input file it was given, and the sha256 of every output
//! it produced. Receipts chain by hash equality: the link job's input object
//! hashes must be compile-job output hashes (`verify_chain`).
//!
//! Everything is hand-encoded (no serde): jobs cross the host→guest boundary
//! as one `write_frame`/`read_frame` blob (risc0's serde would cost a word
//! per byte on multi-MB file trees), and the journal must be a stable,
//! self-describing byte layout anyway.
//!
//! Journal v2 layout (all integers LE, strings are u32-length-prefixed UTF-8):
//!
//! ```text
//! "TCV2"
//! u32 n_ops
//!   per op: u8 tag
//!     1 = compile: str src, str out, u32 n_inc [str..], u32 n_def
//!         [str name, u8 has_value, (str value)]..
//!     2 = link:    u32 n_objs [str..], str out
//! u32 n_inputs   [str path, [u8;32] sha256]..
//! u32 n_outputs  [str path, [u8;32] sha256]..
//! ```
//!
//! Git linkage is deliberately *not* in the journal: the verifier resolves a
//! source path in a repo tree and compares the sha256 of the blob's content,
//! which is the same binding without paying for in-guest SHA-1.

pub const JOURNAL_MAGIC: &[u8; 4] = b"TCV2";
pub const JOB_MAGIC: &[u8; 4] = b"TCJ2";

/// One file in the guest's in-memory filesystem.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct InputFile {
    pub path: String,
    pub data: Vec<u8>,
}

/// A compiler invocation. The flags themselves (`-nostdlib`, `-static`) are
/// baked into the guest and pinned by IMAGE_ID; only path resolution and
/// preprocessor defines are job inputs — and they are committed to the
/// journal, so the verifier sees them.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Op {
    Compile {
        src: String,
        out: String,
        include_dirs: Vec<String>,
        defines: Vec<(String, Option<String>)>,
    },
    Link {
        objs: Vec<String>,
        out: String,
    },
}

impl Op {
    pub fn out_path(&self) -> &str {
        match self {
            Op::Compile { out, .. } => out,
            Op::Link { out, .. } => out,
        }
    }
}

/// What the host sends to the guest (one `write_frame` blob).
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Job {
    pub files: Vec<InputFile>,
    pub ops: Vec<Op>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct FileHash {
    pub path: String,
    pub sha256: [u8; 32],
}

/// The proven statement of one job, committed to (and parsed from) the journal.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Manifest {
    pub ops: Vec<Op>,
    pub inputs: Vec<FileHash>,
    pub outputs: Vec<FileHash>,
}

fn put_str(buf: &mut Vec<u8>, s: &str) {
    buf.extend_from_slice(&(s.len() as u32).to_le_bytes());
    buf.extend_from_slice(s.as_bytes());
}

fn put_ops(buf: &mut Vec<u8>, ops: &[Op]) {
    buf.extend_from_slice(&(ops.len() as u32).to_le_bytes());
    for op in ops {
        match op {
            Op::Compile { src, out, include_dirs, defines } => {
                buf.push(1);
                put_str(buf, src);
                put_str(buf, out);
                buf.extend_from_slice(&(include_dirs.len() as u32).to_le_bytes());
                for d in include_dirs {
                    put_str(buf, d);
                }
                buf.extend_from_slice(&(defines.len() as u32).to_le_bytes());
                for (name, val) in defines {
                    put_str(buf, name);
                    match val {
                        Some(v) => {
                            buf.push(1);
                            put_str(buf, v);
                        }
                        None => buf.push(0),
                    }
                }
            }
            Op::Link { objs, out } => {
                buf.push(2);
                buf.extend_from_slice(&(objs.len() as u32).to_le_bytes());
                for o in objs {
                    put_str(buf, o);
                }
                put_str(buf, out);
            }
        }
    }
}

fn put_hashes(buf: &mut Vec<u8>, files: &[FileHash]) {
    buf.extend_from_slice(&(files.len() as u32).to_le_bytes());
    for f in files {
        put_str(buf, &f.path);
        buf.extend_from_slice(&f.sha256);
    }
}

struct Reader<'a> {
    buf: &'a [u8],
    pos: usize,
}

impl<'a> Reader<'a> {
    fn take(&mut self, n: usize) -> Result<&'a [u8], String> {
        if n > self.buf.len() - self.pos {
            return Err("truncated data".into());
        }
        let s = &self.buf[self.pos..self.pos + n];
        self.pos += n;
        Ok(s)
    }
    fn u8(&mut self) -> Result<u8, String> {
        Ok(self.take(1)?[0])
    }
    fn u32(&mut self) -> Result<u32, String> {
        Ok(u32::from_le_bytes(self.take(4)?.try_into().unwrap()))
    }
    fn str(&mut self) -> Result<String, String> {
        let n = self.u32()? as usize;
        String::from_utf8(self.take(n)?.to_vec()).map_err(|_| "non-utf8 string".into())
    }
    fn ops(&mut self) -> Result<Vec<Op>, String> {
        let n_ops = self.u32()?;
        let mut ops = Vec::new();
        for _ in 0..n_ops {
            ops.push(match self.u8()? {
                1 => {
                    let src = self.str()?;
                    let out = self.str()?;
                    let include_dirs =
                        (0..self.u32()?).map(|_| self.str()).collect::<Result<_, _>>()?;
                    let n_def = self.u32()?;
                    let mut defines = Vec::new();
                    for _ in 0..n_def {
                        let name = self.str()?;
                        let val = if self.u8()? == 1 { Some(self.str()?) } else { None };
                        defines.push((name, val));
                    }
                    Op::Compile { src, out, include_dirs, defines }
                }
                2 => {
                    let objs = (0..self.u32()?).map(|_| self.str()).collect::<Result<_, _>>()?;
                    let out = self.str()?;
                    Op::Link { objs, out }
                }
                t => return Err(format!("unknown op tag {t}")),
            });
        }
        Ok(ops)
    }
    fn hashes(&mut self) -> Result<Vec<FileHash>, String> {
        let n = self.u32()?;
        let mut v = Vec::new();
        for _ in 0..n {
            let path = self.str()?;
            let sha256: [u8; 32] = self.take(32)?.try_into().unwrap();
            v.push(FileHash { path, sha256 });
        }
        Ok(v)
    }
    fn done(&self) -> Result<(), String> {
        if self.pos != self.buf.len() {
            return Err("trailing bytes".into());
        }
        Ok(())
    }
}

impl Job {
    pub fn encode(&self) -> Vec<u8> {
        let mut buf = Vec::new();
        buf.extend_from_slice(JOB_MAGIC);
        buf.extend_from_slice(&(self.files.len() as u32).to_le_bytes());
        for f in &self.files {
            put_str(&mut buf, &f.path);
            buf.extend_from_slice(&(f.data.len() as u32).to_le_bytes());
            buf.extend_from_slice(&f.data);
        }
        put_ops(&mut buf, &self.ops);
        buf
    }

    pub fn decode(bytes: &[u8]) -> Result<Job, String> {
        let mut r = Reader { buf: bytes, pos: 0 };
        if r.take(4)? != JOB_MAGIC {
            return Err("bad job magic".into());
        }
        let n_files = r.u32()?;
        let mut files = Vec::new();
        for _ in 0..n_files {
            let path = r.str()?;
            let len = r.u32()? as usize;
            files.push(InputFile { path, data: r.take(len)?.to_vec() });
        }
        let ops = r.ops()?;
        r.done()?;
        Ok(Job { files, ops })
    }
}

impl Manifest {
    pub fn encode(&self) -> Vec<u8> {
        let mut buf = Vec::new();
        buf.extend_from_slice(JOURNAL_MAGIC);
        put_ops(&mut buf, &self.ops);
        put_hashes(&mut buf, &self.inputs);
        put_hashes(&mut buf, &self.outputs);
        buf
    }

    pub fn decode(bytes: &[u8]) -> Result<Manifest, String> {
        let mut r = Reader { buf: bytes, pos: 0 };
        if r.take(4)? != JOURNAL_MAGIC {
            return Err("bad journal magic (not a TCV2 journal)".into());
        }
        let ops = r.ops()?;
        let inputs = r.hashes()?;
        let outputs = r.hashes()?;
        r.done()?;
        Ok(Manifest { ops, inputs, outputs })
    }
}

/// Framing for guest→host output files (the host reads these from the guest's
/// stdout): u32 count, then per file: str path, u32 len, bytes.
pub fn encode_outputs(files: &[(String, Vec<u8>)]) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.extend_from_slice(&(files.len() as u32).to_le_bytes());
    for (path, data) in files {
        put_str(&mut buf, path);
        buf.extend_from_slice(&(data.len() as u32).to_le_bytes());
        buf.extend_from_slice(data);
    }
    buf
}

pub fn decode_outputs(bytes: &[u8]) -> Result<Vec<(String, Vec<u8>)>, String> {
    let mut r = Reader { buf: bytes, pos: 0 };
    let n = r.u32()?;
    let mut v = Vec::new();
    for _ in 0..n {
        let path = r.str()?;
        let len = r.u32()? as usize;
        v.push((path, r.take(len)?.to_vec()));
    }
    r.done()?;
    Ok(v)
}

/// The verified conclusion of a receipt chain.
#[derive(Debug)]
pub struct ChainSummary {
    /// Every input file that is not itself a proven compile output, deduped:
    /// the source tree the executable provably derives from.
    pub sources: Vec<FileHash>,
    /// Intermediate object files (link inputs, proven compile outputs).
    pub objects: Vec<FileHash>,
    /// The linked executable.
    pub executable: FileHash,
    /// Number of compile ops across the chain.
    pub tu_count: usize,
}

/// Pure hash-chain logic over already-cryptographically-verified manifests:
/// exactly one link op; every object it consumes must be the output of a
/// compile op in some manifest of the chain; source files must not conflict
/// (same path, different content) across jobs.
pub fn verify_chain(manifests: &[Manifest]) -> Result<ChainSummary, String> {
    // outputs of compile ops, by hash
    let mut compile_outputs: Vec<&FileHash> = Vec::new();
    let mut tu_count = 0;
    for m in manifests {
        for op in &m.ops {
            if let Op::Compile { out, .. } = op {
                let h = m
                    .outputs
                    .iter()
                    .find(|o| &o.path == out)
                    .ok_or_else(|| format!("compile output {out} missing from outputs"))?;
                compile_outputs.push(h);
                tu_count += 1;
            }
        }
    }

    // exactly one link op across the chain
    let mut link: Option<(&Manifest, &Vec<String>, &str)> = None;
    for m in manifests {
        for op in &m.ops {
            if let Op::Link { objs, out } = op {
                if link.is_some() {
                    return Err("more than one link op in chain".into());
                }
                link = Some((m, objs, out));
            }
        }
    }
    let (link_m, objs, link_out) = link.ok_or("no link op in chain")?;

    let executable = link_m
        .outputs
        .iter()
        .find(|o| o.path == link_out)
        .ok_or("link output missing from outputs")?
        .clone();

    // every linked object must be a proven compile output (same-job outputs
    // cover the single-job compile+link case; cross-job objects arrive as
    // inputs of the link job)
    let mut objects = Vec::new();
    for obj in objs {
        let h = link_m
            .outputs
            .iter()
            .chain(link_m.inputs.iter())
            .find(|f| &f.path == obj)
            .ok_or_else(|| format!("linked object {obj} not among link job files"))?;
        if !compile_outputs.iter().any(|c| c.sha256 == h.sha256) {
            return Err(format!("linked object {obj} is not a proven compile output"));
        }
        objects.push(h.clone());
    }

    // sources = inputs that are not proven compile outputs; reject two
    // different contents claiming the same path
    let mut sources: Vec<FileHash> = Vec::new();
    for m in manifests {
        for f in &m.inputs {
            if compile_outputs.iter().any(|c| c.sha256 == f.sha256) {
                continue;
            }
            match sources.iter().find(|s| s.path == f.path) {
                Some(s) if s.sha256 != f.sha256 => {
                    return Err(format!("conflicting contents for input path {}", f.path));
                }
                Some(_) => {}
                None => sources.push(f.clone()),
            }
        }
    }

    Ok(ChainSummary { sources, objects, executable, tu_count })
}

pub fn hex(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02x}")).collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn h(b: u8) -> [u8; 32] {
        [b; 32]
    }

    fn compile_manifest() -> Manifest {
        Manifest {
            ops: vec![Op::Compile {
                src: "src/a.c".into(),
                out: "obj/a.o".into(),
                include_dirs: vec!["include".into()],
                defines: vec![("_XOPEN_SOURCE".into(), Some("700".into())), ("CRT".into(), None)],
            }],
            inputs: vec![
                FileHash { path: "src/a.c".into(), sha256: h(1) },
                FileHash { path: "include/x.h".into(), sha256: h(2) },
            ],
            outputs: vec![FileHash { path: "obj/a.o".into(), sha256: h(3) }],
        }
    }

    fn link_manifest() -> Manifest {
        Manifest {
            ops: vec![Op::Link { objs: vec!["obj/a.o".into()], out: "hello".into() }],
            inputs: vec![FileHash { path: "obj/a.o".into(), sha256: h(3) }],
            outputs: vec![FileHash { path: "hello".into(), sha256: h(4) }],
        }
    }

    #[test]
    fn manifest_roundtrip() {
        for m in [compile_manifest(), link_manifest()] {
            assert_eq!(Manifest::decode(&m.encode()).unwrap(), m);
        }
    }

    #[test]
    fn job_roundtrip() {
        let job = Job {
            files: vec![
                InputFile { path: "src/a.c".into(), data: b"int main(){}".to_vec() },
                InputFile { path: "include/x.h".into(), data: vec![] },
            ],
            ops: compile_manifest().ops.clone(),
        };
        assert_eq!(Job::decode(&job.encode()).unwrap(), job);
    }

    #[test]
    fn outputs_roundtrip() {
        let outs = vec![("obj/a.o".to_string(), vec![1u8, 2, 3]), ("hello".to_string(), vec![])];
        assert_eq!(decode_outputs(&encode_outputs(&outs)).unwrap(), outs);
    }

    #[test]
    fn decode_rejects_garbage() {
        assert!(Manifest::decode(b"TCV2").is_err());
        assert!(Manifest::decode(b"NOPE\0\0\0\0").is_err());
        let mut enc = compile_manifest().encode();
        enc.push(0);
        assert!(Manifest::decode(&enc).is_err());
    }

    #[test]
    fn chain_two_jobs() {
        let s = verify_chain(&[compile_manifest(), link_manifest()]).unwrap();
        assert_eq!(s.tu_count, 1);
        assert_eq!(s.executable.sha256, h(4));
        assert_eq!(s.sources.len(), 2);
        assert_eq!(s.objects.len(), 1);
    }

    #[test]
    fn chain_single_job() {
        // compile+link inside one job: object is an output, not an input
        let m = Manifest {
            ops: vec![
                Op::Compile {
                    src: "src/a.c".into(),
                    out: "obj/a.o".into(),
                    include_dirs: vec![],
                    defines: vec![],
                },
                Op::Link { objs: vec!["obj/a.o".into()], out: "out.elf".into() },
            ],
            inputs: vec![FileHash { path: "src/a.c".into(), sha256: h(1) }],
            outputs: vec![
                FileHash { path: "obj/a.o".into(), sha256: h(3) },
                FileHash { path: "out.elf".into(), sha256: h(4) },
            ],
        };
        let s = verify_chain(&[m]).unwrap();
        assert_eq!(s.sources.len(), 1);
        assert_eq!(s.executable.path, "out.elf");
    }

    #[test]
    fn chain_rejects_foreign_object() {
        let mut link = link_manifest();
        link.inputs[0].sha256 = h(9); // not produced by any compile op
        let err = verify_chain(&[compile_manifest(), link]).unwrap_err();
        assert!(err.contains("not a proven compile output"));
    }

    #[test]
    fn chain_rejects_conflicting_sources() {
        let c1 = compile_manifest();
        let mut c2 = compile_manifest();
        c2.ops = vec![Op::Compile {
            src: "src/b.c".into(),
            out: "obj/b.o".into(),
            include_dirs: vec![],
            defines: vec![],
        }];
        c2.inputs = vec![
            FileHash { path: "src/b.c".into(), sha256: h(5) },
            FileHash { path: "include/x.h".into(), sha256: h(6) }, // != h(2)
        ];
        c2.outputs = vec![FileHash { path: "obj/b.o".into(), sha256: h(7) }];
        let mut link = link_manifest();
        link.ops = vec![Op::Link {
            objs: vec!["obj/a.o".into(), "obj/b.o".into()],
            out: "hello".into(),
        }];
        link.inputs.push(FileHash { path: "obj/b.o".into(), sha256: h(7) });
        assert!(verify_chain(&[c1, c2, link]).unwrap_err().contains("conflicting"));
    }

    #[test]
    fn chain_requires_exactly_one_link() {
        assert!(verify_chain(&[compile_manifest()]).is_err());
        assert!(verify_chain(&[compile_manifest(), link_manifest(), link_manifest()]).is_err());
    }
}
