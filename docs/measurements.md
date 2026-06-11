# Measurements

All measured 2026-06-10/11. Local CPU = 8-core AVX-512, no GPU. GPU = rented
vast.ai RTX 4090 @ ~$0.39/hr via `cloud/prove-remote.sh`.

## Cost model (validated, use for planning)

```
guest cycles ≈ 1.2M (job startup) + ~450 × bytes *parsed per TU*
               (single self-contained file: ≈ source bytes, ±10% by density;
                multi-TU: each TU re-parses its include closure — for musl
                TUs that's ~100–300 KB of headers ⇒ ~7–9M cycles per TU)
1 segment    = 2^20 cycles ≈ 2.3 KB of self-contained source
GPU(4090)    ≈ 12 s fixed + ~1.55 s/segment  ⇒  ~12 min and ~$0.08 per MB of source
CPU(8-core)  ≈ ~550 s/segment (composite)    ⇒  ~85–350× slower than one 4090
groth16 wrap : constant ~160–230 s per receipt (CPU + podman rapidsnark)
verification : constant — 665 B receipt, ~5 ms, any program size
execution    : ~25 MHz emulated (compilation output available in seconds)
```

## Proof modes, hello.c (619 B, 1.34M user cycles, 2 segments), local CPU

| receipt   | proof size | proving | verification |
|-----------|-----------:|--------:|-------------:|
| composite |     563 KB |  1101 s |        41 ms |
| succinct  |     223 KB |  1258 s |        18 ms |
| groth16   |  **665 B** |  1557 s |     **4 ms** |

## GPU size trend (succinct proving)

| source | size | user cycles | segments | GPU time |
|---|---:|---:|---:|---:|
| hello.c | 619 B | 1.34 M | 2 | 14.9 s |
| bench_small.c | 5.9 KB | 3.78 M | 5 | 19.2 s |
| bench_medium.c | 24.9 KB | 12.2 M | 14 | 33.1 s |
| bench_large.c | 103 KB | 46.9 M | 50 | 89.6 s |

Linear fit: ~12 s + 1.55 s/segment. (Cold-instance runs include CUDA
warmup; warm-instance runs came in at ~12 s for 2–3 segments.)

## Realistic kernels (GPU, warm instance)

Outputs computed by the *proven* binaries, checked correct:

| program | size | cycles | GPU | output |
|---|---:|---:|---:|---|
| fib.c | 951 B | 1.49 M | 12.0 s | fib(90)=2880067194370816120 |
| primes.c | 1.2 KB | 1.60 M | 12.0 s | count=78498 sum=37550402023 (primes < 10⁶) |
| sort.c | 1.8 KB | 1.82 M | 12.9 s | sorted=1 + deterministic FNV digest |

Realistic code structure (loops, recursion, arrays) barely differs from
synthetic code per byte — source size drives cost, not code shape.

## Multi-TU: musl hello (examples/musl-hello, measured 2026-06-11)

44 TUs (hello.c + crt1 + 42 musl/tcc-runtime TUs from the printf closure),
115 input files, batch 16 ⇒ 3 compile jobs + 1 link job, 41 374-byte
static executable that prints `hello, musl 42`. Byte-identical output
across separate `--only` runs, dev and GPU executions (deterministic).

| job | TUs | user cycles | segments |
|---|---:|---:|---:|
| job000 | 16 | 142.5 M | 176 |
| job001 | 16 | 114.3 M | 141 |
| job002 | 12 | 65.8 M | 81 |
| link | 1 op | 1.8 M | 3 |
| total | 44 | 324.4 M | 401 |

**Measured on a rented 4090** ($0.40/hr, `prove-remote.sh --build`): the
whole chain proved succinct in **740 s wall** (the 12 s + 1.55 s/segment
model predicts 634 s of pure proving — the rest is per-job executor +
serialization overhead). The four local groth16 wraps took 160–177 s
each; end-to-end cold instance → verified groth16 chain ≈ 29 min wall.
Exact billed cost (`vastai show invoices`): **$0.174** for the instance
(GPU $0.167 + storage $0.007 — storage bills the requested 48 GB for the
full lifetime, including the ~5 min image pull/boot). Parallelizing the
3 compile jobs across instances (`--only`) would cut GPU wall to the
largest job (~285 s) at the price of paying boot overhead per instance.

Per-TU cost is dominated by the include closure (~7–9 M cycles ≈ 8–10
segments per musl TU), not TU body size — batching amortizes only the
1.2 M-cycle job startup. The link job is nearly free (3 segments).
Journals are no longer 84 bytes: each commits its full manifest, so the
groth16 receipts are journal-dominated (8.6–18.5 KB instead of 665 B);
the 4-receipt embedded attestation is ~99 KB and verifies in ~17 ms.

## Other measured facts

- Segments serialize to ~250 KB each (≈12.7 MB for the 50-segment program)
  — shipping segments to remote provers is bandwidth-trivial, though
  shipping the source (KBs) and re-executing remotely is simpler.
- End-to-end cold-instance cloud run (boot → pull 20 GB image → prove →
  fetch → local wrap → verify): ~8 min wall for hello.c, of which 17 s is
  GPU proving.
- CI bake: ~60 min cold, **~15 min** with the cargo-chef registry cache
  (kernel layer rebuilt only when dependencies change).
- IMAGE_ID reproducibility: docker bake on GitHub runners and a provisioned
  vast.ai instance produced **identical** guest ids independently.
- Whole session's GPU spend, including failed-host debugging: ~$1.40.

## Extrapolations (single 4090; parallelize across GPUs ≈ linearly)

| target | source | est. GPU time | est. cost | blocker |
|---|---:|---:|---:|---|
| DOOM (~38K LOC, ~60 TUs) | ~1.5 MB | ~1–1.5 h serial | ~25–55¢ | build manifest + enough musl closure for its I/O (multi-file inputs: done) |
| musl libc, all ~1262 TUs | ~5 MB | ~3.5 h serial | ~$1.40 | 37 TUs fail TCC (x87 asm constraints `x`/`t`, expl.s syntax) |
| sqlite amalgamation | 8.4 MB | ~105 min | ~70¢ | TCC heap for one giant TU vs rv32 address space |
| gcc | 50+ MB | (~10 h if it compiled) | ~$4 | TCC can't compile C++; memory needs 64-bit guest |

(Multi-TU estimates use the measured per-TU include-closure cost, not raw
source size — that is why "all of musl" got *more* expensive than the old
naive ~45¢ extrapolation while parallelizing better: every job is
independent, so wall time divides by the number of GPUs.)

Cycles/byte is a property of the *compiler*, not of proving: gcc/clang -O0
≈ 10× TCC's slope, -O2 ≈ 30–100× (see `docs/compiler-economics.md`).
