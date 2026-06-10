# Prebaked vast.ai image: CUDA prover for tcc-verified, ready at boot.
# Built by .github/workflows/bake-prover-image.yml; the guest compiled in
# here IS the canonical compiler (CANONICAL_IMAGE_ID pins it).
#
# cargo-chef layer split: dependency compilation (including the ~40-80 min
# CUDA kernel builds in risc0-circuit-*-sys) lives in the `cook` layer keyed
# only on Cargo.toml/Cargo.lock, and is reused from the registry build cache
# across bakes. Source-only changes rebuild in minutes.
#
# The base must be a -devel image: risc0's CUDA kernels compile with nvcc.
FROM nvidia/cuda:12.6.3-devel-ubuntu24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl git ca-certificates build-essential cmake ninja-build \
        pkg-config libssl-dev openssh-server rsync jq protobuf-compiler \
        clang libclang-dev \
    && rm -rf /var/lib/apt/lists/*

# host Rust toolchain
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH=/root/.cargo/bin:/root/.risc0/bin:$PATH

# RISC Zero toolchains, pinned: the canonical guest build environment
RUN curl -L https://risczero.com/install | bash \
    && rzup install rust 1.94.1 \
    && rzup install cpp 2024.1.5

# CUDA arch must be explicit: risc0-build-kernel defaults to -arch=native,
# which probes a GPU the CI runner doesn't have (and setting this var is the
# upstream-intended way to disable that). sm_89 = RTX 4090; the embedded
# compute_89 PTX lets newer GPUs (e.g. 5090) JIT. Older GPUs need a rebake.
ENV NVCC_APPEND_FLAGS="-gencode arch=compute_89,code=[sm_89,compute_89]"

RUN cargo install cargo-chef --locked

# ---- plan: distill the dependency graph into a recipe ----
FROM base AS planner
WORKDIR /opt/tcc-verified
COPY . .
RUN cargo chef prepare --recipe-path recipe.json

# ---- cook: build dependencies only (the expensive, cacheable layer) ----
FROM base AS builder
WORKDIR /opt/tcc-verified
COPY --from=planner /opt/tcc-verified/recipe.json recipe.json
RUN cargo chef cook --release -p host --features cuda --recipe-path recipe.json

# ---- build: workspace crates + the canonical guest (fast) ----
COPY . .
RUN cargo build --release -p host --features cuda
