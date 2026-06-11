#!/usr/bin/env bash
# Prove a compilation on a rented vast.ai GPU, finish locally.
#
#   cloud/prove-remote.sh <source.c> [options]
#
# Default flow uses the CI-baked image (ghcr): the instance boots with the
# prover prebuilt, the source is shipped as a runtime input, and the receipt
# comes back proven by the CANONICAL guest (id in CANONICAL_IMAGE_ID, baked
# by .github/workflows/bake-prover-image.yml). No remote build: source files
# are inputs and do not affect the IMAGE_ID.
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
#   --image IMG       docker image                (default: ghcr.io/nikicat/tcc-verified-prover:latest)
#                     stock images (slow path, provisions + builds on boot):
#                     --image nvidia/cuda:12.6.3-devel-ubuntu24.04
#   --instance ID     reuse an existing instance (skip search/create)
#   --keep            don't destroy the instance afterwards
#   --skip-wrap       stop after fetching the succinct receipt
#
# NOTE: vastai CLI output formats drift between versions; the jq paths below
# are for vastai 0.5.x.
set -euo pipefail
cd "$(dirname "$0")/.."

SRC=""
GPU="RTX_4090"
MAX_DPH="0.60"
IMAGE="ghcr.io/nikicat/tcc-verified-prover:latest"
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

# progress reporting: every stage line goes, timestamped and unbuffered, to
# $PROGRESS_FILE — `tail -f` it from anywhere regardless of stdout buffering
T_START=$(date +%s)
PROGRESS_FILE="${PROGRESS_FILE:-/tmp/prove-remote-progress.log}"
: > "$PROGRESS_FILE"
log() {
    local msg="[prove-remote +$(($(date +%s) - T_START))s] $*"
    printf '\033[1;34m%s\033[0m\n' "$msg"
    printf '%s\n' "$msg" >> "$PROGRESS_FILE"
}
log "progress log: $PROGRESS_FILE"

# deterministic single-key auth: agents with many (wrong) keys trip hosts'
# MaxAuthTries ("Too many authentication failures"), and one auth attempt
# also exposes hosts where vast's key injection failed, immediately
SSH_KEY="${VAST_SSH_KEY:-$HOME/.ssh/id_ed25519}"
SSH_BASE=(-F /dev/null -o IdentityAgent=none -o IdentitiesOnly=yes -i "$SSH_KEY"
          -o StrictHostKeyChecking=accept-new -o ConnectTimeout=10)

# ---------- 1. instance ----------
CREATED=0
if [ -z "$INSTANCE" ]; then
    log "searching offers: $GPU, <= \$$MAX_DPH/hr, reliable, fast net..."
    OFFER=$(vastai search offers \
        "gpu_name=$GPU num_gpus=1 rentable=true reliability>0.99 cuda_vers>=12.6 inet_down>200 disk_space>40 dph<=$MAX_DPH" \
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
log "waiting for instance to start (baked image pull ~3-5 min)..."
STATUS=""
for i in $(seq 1 90); do
    INFO=$(vastai show instance "$INSTANCE" --raw)
    # API reports progress inconsistently: actual_status may stay null while
    # cur_state says running; trust either, fall through to the ssh probe.
    STATUS=$(jq -r '.actual_status // .cur_state // empty' <<<"$INFO")
    [ "$STATUS" = "running" ] && break
    [ $((i % 6)) = 0 ] && log "still waiting for instance (status: ${STATUS:-none})..."
    sleep 10
done
[ "$STATUS" = "running" ] || { echo "instance never reached running state: $(jq -c '{actual_status,cur_state,status_msg}' <<<"$INFO")"; exit 1; }
# ssh_host/ssh_port can stay null in the API well after status=running —
# re-poll until assigned, then probe (capturing them once loses the race)
SSH_OK=0
for i in $(seq 1 140); do
    INFO=$(vastai show instance "$INSTANCE" --raw)
    SSH_HOST=$(jq -r '.ssh_host // empty' <<<"$INFO")
    SSH_PORT=$(jq -r '.ssh_port // empty' <<<"$INFO")
    if [ -n "$SSH_HOST" ] && [ -n "$SSH_PORT" ] && [ "$SSH_PORT" != "null" ]; then
        SSH=(ssh -p "$SSH_PORT" "${SSH_BASE[@]}" "root@$SSH_HOST")
        "${SSH[@]}" true 2>/dev/null && { SSH_OK=1; break; }
    fi
    [ $((i % 12)) = 0 ] && log "still waiting for ssh (port: ${SSH_PORT:-unassigned}, likely image pull)..."
    sleep 5
done
[ "$SSH_OK" = 1 ] || { echo "ssh never came up (port assigned: ${SSH_PORT:-no})"; exit 1; }
log "ssh: root@$SSH_HOST:$SSH_PORT"

# sanity: some hosts rent out containers without a working CUDA device
"${SSH[@]}" 'nvidia-smi -L' >/dev/null 2>&1 || { echo "FATAL: no CUDA device visible on the rented host — destroy and re-rent"; exit 1; }

# ---------- 3. prebaked or provision ----------
if "${SSH[@]}" '[ -x /opt/tcc-verified/target/release/host ]' 2>/dev/null; then
    log "prebaked image detected, no remote build needed"
else
    log "stock image: rsyncing project + provisioning (~35 min first boot)..."
    rsync -az --delete --exclude target --exclude out --exclude .git \
        -e "ssh -p $SSH_PORT ${SSH_BASE[*]}" \
        ./ "root@$SSH_HOST:/opt/tcc-verified/"
    "${SSH[@]}" "bash /opt/tcc-verified/cloud/provision.sh" | tail -1
fi

# canonical id: pinned in-repo by CI bake; fall back to local build's id
EXPECTED_ID=$(cat CANONICAL_IMAGE_ID 2>/dev/null || ./target/release/host --image-id 2>/dev/null || true)
REMOTE_ID=$("${SSH[@]}" 'cd /opt/tcc-verified && ./target/release/host --image-id')
if [ -n "$EXPECTED_ID" ] && [ "$REMOTE_ID" != "$EXPECTED_ID" ]; then
    echo "FATAL: remote IMAGE_ID $REMOTE_ID != expected $EXPECTED_ID"
    echo "(is the instance running an outdated baked image?)"
    exit 1
fi
log "image id: $REMOTE_ID"

# ---------- 4. ship source, prove on GPU ----------
log "proving $SRC (succinct, GPU)..."
scp -P "$SSH_PORT" "${SSH_BASE[@]}" "$SRC" "root@$SSH_HOST:/tmp/input.c"
T0=$(date +%s)
"${SSH[@]}" "cd /opt/tcc-verified && RISC0_PROVER=local RUST_LOG=info \
    ./target/release/host /tmp/input.c --mode succinct --out-dir /tmp/out" \
    2>&1 | tee -a "$PROGRESS_FILE"
T1=$(date +%s)
log "remote GPU proving wall time: $((T1 - T0))s"

# ---------- 5. fetch ----------
RDIR="out/remote-$(basename "$SRC" .c)"
mkdir -p "$RDIR"
scp -P "$SSH_PORT" "${SSH_BASE[@]}" \
    "root@$SSH_HOST:/tmp/out/receipt.succinct.bin" \
    "root@$SSH_HOST:/tmp/out/hello" "$RDIR/"
log "fetched: $RDIR/receipt.succinct.bin ($(stat -c%s "$RDIR/receipt.succinct.bin") bytes) + binary"

# ---------- 6. local groth16 wrap + verify against the canonical id ----------
VERIFY_ARGS=()
[ -n "$EXPECTED_ID" ] && VERIFY_ARGS=(--image-id "$EXPECTED_ID")
if [ "$SKIP_WRAP" = 0 ]; then
    log "wrapping to groth16 locally (podman)..."
    DOCKER_HOST="${DOCKER_HOST:-unix:///run/user/$(id -u)/podman/podman.sock}" \
        RISC0_PROVER=local ./target/release/host --wrap "$RDIR/receipt.succinct.bin" --out-dir "$RDIR"
    ./target/release/verifier "$RDIR/receipt.groth16.bin" "$RDIR/hello" "$SRC" "${VERIFY_ARGS[@]}"
else
    ./target/release/verifier "$RDIR/receipt.succinct.bin" "$RDIR/hello" "$SRC" "${VERIFY_ARGS[@]}"
fi

log "done. artifacts in $RDIR/"
