#!/usr/bin/env bash
# Provision a stock CUDA devel image on a vast.ai instance for tcc-verified
# proving. Used as the no-registry alternative to the prebaked Dockerfile:
# first boot costs ~30-40 min of build time (~$0.20 on a 4090), later runs
# rebuild incrementally.
#
# Run on the instance (prove-remote.sh does this automatically when needed).
set -euxo pipefail

if ! command -v cc >/dev/null || ! command -v protoc >/dev/null || ! command -v clang >/dev/null; then
    apt-get update
    apt-get install -y --no-install-recommends \
        curl git ca-certificates build-essential cmake ninja-build \
        pkg-config libssl-dev rsync jq protobuf-compiler clang libclang-dev
fi

if [ ! -x "$HOME/.cargo/bin/cargo" ]; then
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
fi
. "$HOME/.cargo/env"

if [ ! -x "$HOME/.risc0/bin/rzup" ]; then
    curl -L https://risczero.com/install | bash
fi
export PATH="$HOME/.risc0/bin:$PATH"
# pinned: must match the local machine for IMAGE_ID-identical guest builds
rzup install rust 1.94.1 || true
rzup install cpp 2024.1.5 || true

cd /opt/tcc-verified
cargo build --release -p host --features cuda
RISC0_PROVER=local ./target/release/host --image-id
