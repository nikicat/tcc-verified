# Prebaked vast.ai image: CUDA prover for tcc-verified, ready at boot.
#
# Build & push (only needed if you want fast instance boots; the no-registry
# alternative is cloud/provision.sh on a stock image):
#   docker build -f cloud/Dockerfile -t <you>/tcc-verified-prover:latest .
#   docker push <you>/tcc-verified-prover:latest
#
# The base must be a -devel image: risc0's CUDA kernels compile with nvcc.
FROM nvidia/cuda:12.6.3-devel-ubuntu24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl git ca-certificates build-essential cmake ninja-build \
        pkg-config libssl-dev openssh-server rsync jq protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

# host Rust toolchain
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH=/root/.cargo/bin:/root/.risc0/bin:$PATH

# RISC Zero toolchains, pinned to the same versions as the local machine so
# the guest build is bit-identical => same IMAGE_ID => receipts verify locally
RUN curl -L https://risczero.com/install | bash \
    && rzup install rust 1.94.1 \
    && rzup install cpp 2024.1.5

# prebuild the project to bake compile caches into the image (~30 min once;
# includes the CUDA kernel build). prove-remote.sh rsyncs fresh sources over
# this and rebuilds incrementally in seconds.
COPY . /opt/tcc-verified
WORKDIR /opt/tcc-verified
RUN cargo build --release -p host --features cuda
