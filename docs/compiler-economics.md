# Compiler economics — why TCC, what gcc/clang would cost

The zkVM bill is linear in the cycles the compiler executes; choosing the
compiler is choosing the slope. This doc explains the slopes.

## Why TCC is the right first compiler

TCC is a true one-pass compiler: no AST, no IR — code is emitted during
parsing with a small value-stack abstraction, one function in memory at a
time, near-zero optimization. That is ~450 cycles/byte in our measurements
and a near-streaming memory footprint, which matters twice in a zkVM:
cycles are money and the rv32im guest address space (~192 MB usable) is
small. TCC also has an in-memory ELF linker (source → runnable static
binary inside one process — the proof covers the *whole* path) and is
portable enough to cross-compile to riscv32im unmodified.

The price: TCC's output runs ~2–3× slower than gcc -O2. For provenance
use-cases that want production-grade binaries, a better-optimizing proven
compiler is the obvious future step — see the QBE note below.

## Where gcc's 30–100× goes

1. **Pipeline depth, even at -O0** (~10×): gcc lowers through
   GENERIC → GIMPLE(SSA) → RTL, materializing the whole TU in successive
   IRs with full type/alias/location bookkeeping. You pay for the highway
   even driving slowly.
2. **Optimization passes** (~3–5× more at -O2): ~200 pass instances — SSA
   construction, GVN, inlining, loop transforms, IRA/LRA register
   allocation, scheduling.
3. **Multilanguage heritage: minor for C.** GENERIC-as-common-IR adds
   indirection, but plain C does not pay C++'s frontend costs (templates,
   overload resolution). C-only gcc would be tens of percent faster, not
   multiples.
4. **Memory**: 50–100× source size in RAM (SSA + alias + DWARF), GBs on
   large TUs — **exceeds the rv32 guest address space**; proven-gcc
   realistically waits for a 64-bit-guest zkVM, independent of cycle cost.

## clang/LLVM

Same architectural class, by design — LLVM's product *is* the reusable SSA
IR and pass library. Typed retained AST → LLVM IR → SelectionDAG/MIR → MC.
Throughput and memory within small factors of gcc at comparable -O levels.
Porting clang as a guest is also far harder than TCC (C++, threads, huge
binaries).

## The interesting middle: QBE-class backends

cproc + QBE (or similar small-SSA compilers): real optimization (~70% of
gcc -O2 output quality) at an estimated 5–10× TCC's cycle slope, small-C
codebase, modest memory. This is the sweet spot for "proven builds whose
binaries aren't toys" and the natural second compiler image for this
project. chibicc is even easier to port but emits asm text (needs a proven
assembler step; see the CosmicTurtle paper).

## Rules of thumb

| compiler | cycles/byte vs TCC | proven cost per MB (4090) | guest feasibility |
|---|---:|---:|---|
| TCC | 1× | ~$0.08, ~12 min | done (this repo) |
| cproc/QBE | ~5–10× | ~$0.5–1 | very plausible port |
| gcc/clang -O0 | ~10× | ~$1 | hard port, memory tight |
| gcc/clang -O2 | ~30–100× | ~$3–8 | blocked: memory (needs 64-bit guest), port effort |
