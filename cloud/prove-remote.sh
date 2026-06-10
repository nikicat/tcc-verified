#!/usr/bin/env bash
# Prove a compilation on a rented vast.ai GPU, finish locally.
#
#   cloud/prove-remote.sh <source.c> [options]
#
# Flow: rent cheapest matching GPU -> rsync repo -> remote: build (incremental)
# + prove succinct on GPU -> fetch receipt (223 KB) + binary -> local: wrap to
# groth16 via podman -> verify -> destroy instance.
#
# Receipts are self-verifying: a malicious/flaky host can only fail to
# deliver, never forge - which is why the cheap spot market is safe here.
#
# Prerequisites:
#   vastai set api-key <KEY>          # account: https://cloud.vast.ai
#   ssh key attached to the account (web UI: Account > SSH Keys)
#   systemctl --user start podman.socket   (for the local groth16 wrap)
#
# Options:
#   --gpu NAME        GPU model filter            (default: RTX_4090)
#   --max-dph N       max $/hr                    (default: 0.60)
#   --image IMG       docker image                (default: nvidia/cuda:12.6.3-devel-ubuntu24.04)
#                     prebaked (boots ready, see .github/workflows/bake-prover-image.yml):
#                     --image ghcr.io/nikicat/tcc-verified-prover:latest
#   --instance ID     reuse an existing instance (skip search/create)
#   --keep            don't destroy the instance afterwards
#   --skip-wrap       stop after fetching the succinct receipt
#
# NOTE: vastai CLI output formats drift between versions; the jq paths below
# are for vastai 0.5.x. If search/create break, check `vastai search offers
# --help` first.
set -euo pipefail
cd "$(dirname "$0")/.."

SRC=""
GPU="RTX_4090"
MAX_DPH="0.60"
IMAGE="nvidia/cuda:12.6.3-devel-ubuntu24.04"
INSTANCE=""
KEEP=0
SKIP_WRAP=0
while [ $# -gt 0 ]; do
    case "$1" in
        --gpu) GPU="$2"; shift 2 ;;
        --max-dph) MAX_DPH="$2"; shift 2 ;;
        --image) IMAGE="$2"; shift 2 ;;
        --instance) INSTANCE="$2"; shift 2 ;;
        --keep) KEEP=1; shift ;;
        --skip-wrap) SKIP_WRAP=1; shift ;;
        *) SRC="$1"; shift ;;
    esac
done
[ -n "$SRC" ] || { echo "usage: $0 <source.c> [--gpu RTX_4090] [--max-dph 0.60] [--keep]"; exit 2; }
[ -f "$SRC" ] || { echo "no such source: $SRC"; exit 2; }

log() { printf '\033[1;34m[prove-remote]\033[0m %s\n' "$*"; }

# ---------- 1. instance ----------
CREATED=0
if [ -z "$INSTANCE" ]; then
    log "searching offers: $GPU, <= \$$MAX_DPH/hr, reliable, fast net..."
    OFFER=$(vastai search offers \
        "gpu_name=$GPU num_gpus=1 rentable=true reliability>0.99 cuda_vers>=12.4 inet_down>200 disk_space>40 dph<=$MAX_DPH" \
        -o dph --raw | jq '.[0]')
    [ "$OFFER" != "null" ] || { echo "no offers matched"; exit 1; }
    OFFER_ID=$(jq -r '.id' <<<"$OFFER")
    log "cheapest offer: id=$OFFER_ID $(jq -r '"\(.gpu_name) @ $\(.dph_total)/hr, \(.inet_down)Mbps down, reliability \(.reliability2 // .reliability)"' <<<"$OFFER")"

    log "creating instance (image: $IMAGE)..."
    CREATE_OUT=$(vastai create instance "$OFFER_ID" --image "$IMAGE" --disk 48 --ssh --direct --raw)
    INSTANCE=$(jq -r '.new_contract' <<<"$CREATE_OUT" 2>/dev/null) || true
    [ -n "$INSTANCE" ] && [ "$INSTANCE" != "null" ] || { echo "create failed, raw output:"; echo "$CREATE_OUT"; exit 1; }
    CREATED=1
    log "instance id: $INSTANCE"
fi

destroy() {
    if [ "$KEEP" = 0 ] && [ "$CREATED" = 1 ]; then
        log "destroying instance $INSTANCE"
        vastai destroy instance "$INSTANCE" || echo "WARNING: destroy failed - check https://cloud.vast.ai to avoid charges!"
    elif [ -n "$INSTANCE" ]; then
        log "keeping instance $INSTANCE (remember: it bills until destroyed)"
    fi
}
trap destroy EXIT

# ---------- 2. wait for ssh ----------
log "waiting for instance to start..."
for i in $(seq 1 60); do
    INFO=$(vastai show instance "$INSTANCE" --raw)
    # API reports progress inconsistently: actual_status may stay null while
    # cur_state says running; trust either, fall through to the ssh probe.
    STATUS=$(jq -r '.actual_status // .cur_state // empty' <<<"$INFO")
    [ "$STATUS" = "running" ] && break
    sleep 10
done
[ "$STATUS" = "running" ] || { echo "instance never reached running state: $(jq -c '{actual_status,cur_state,status_msg}' <<<"$INFO")"; exit 1; }
SSH_HOST=$(jq -r '.ssh_host' <<<"$INFO")
SSH_PORT=$(jq -r '.ssh_port' <<<"$INFO")
SSH=(ssh -p "$SSH_PORT" -o StrictHostKeyChecking=accept-new -o ConnectTimeout=10 "root@$SSH_HOST")
log "ssh: root@$SSH_HOST:$SSH_PORT"
for i in $(seq 1 30); do "${SSH[@]}" true 2>/dev/null && break; sleep 5; done

# ---------- 3. sync + provision + build ----------
log "rsyncing project..."
rsync -az --delete --exclude target --exclude out --exclude .git \
    -e "ssh -p $SSH_PORT -o StrictHostKeyChecking=accept-new" \
    ./ "root@$SSH_HOST:/opt/tcc-verified/"

log "provisioning + building (first boot ~30-40 min, warm instance ~seconds)..."
"${SSH[@]}" "bash /opt/tcc-verified/cloud/provision.sh" | tail -1 > /tmp/remote-image-id
REMOTE_ID=$(cat /tmp/remote-image-id)
LOCAL_ID=$(./target/release/host --image-id 2>/dev/null || true)
if [ -n "$LOCAL_ID" ] && [ "$REMOTE_ID" != "$LOCAL_ID" ]; then
    echo "FATAL: remote IMAGE_ID $REMOTE_ID != local $LOCAL_ID (toolchain drift?)"
    exit 1
fi
log "image id match: $REMOTE_ID"

# ---------- 4. prove on GPU ----------
SRC_REL=$(realpath --relative-to=. "$SRC")
case "$SRC_REL" in ../*) echo "source must live inside the repo (rsync ships the repo)"; exit 2 ;; esac
log "proving $SRC_REL (succinct, GPU)..."
T0=$(date +%s)
"${SSH[@]}" "cd /opt/tcc-verified && RISC0_PROVER=local RUST_LOG=info \
    ./target/release/host '$SRC_REL' --mode succinct --out-dir out" \
    | tee /tmp/remote-prove.log
T1=$(date +%s)
log "remote GPU proving wall time: $((T1 - T0))s"

# ---------- 5. fetch ----------
RDIR="out/remote-$(basename "$SRC" .c)"
mkdir -p "$RDIR"
scp -P "$SSH_PORT" -o StrictHostKeyChecking=accept-new \
    "root@$SSH_HOST:/opt/tcc-verified/out/receipt.succinct.bin" \
    "root@$SSH_HOST:/opt/tcc-verified/out/hello" "$RDIR/"
log "fetched: $RDIR/receipt.succinct.bin ($(stat -c%s "$RDIR/receipt.succinct.bin") bytes) + binary"

# ---------- 6. local groth16 wrap + verify ----------
if [ "$SKIP_WRAP" = 0 ]; then
    log "wrapping to groth16 locally (podman)..."
    DOCKER_HOST="${DOCKER_HOST:-unix:///run/user/$(id -u)/podman/podman.sock}" \
        RISC0_PROVER=local ./target/release/host --wrap "$RDIR/receipt.succinct.bin" --out-dir "$RDIR"
    ./target/release/verifier "$RDIR/receipt.groth16.bin" "$RDIR/hello" "$SRC"
else
    ./target/release/verifier "$RDIR/receipt.succinct.bin" "$RDIR/hello" "$SRC"
fi

log "done. artifacts in $RDIR/"
