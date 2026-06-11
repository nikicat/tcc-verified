#!/usr/bin/env bash
# Prove a compilation on a rented vast.ai GPU, finish locally.
#
#   cloud/prove-remote.sh <source.c> [options]
#   cloud/prove-remote.sh --build <build.json> [options]
#
# Default flow uses the CI-baked image (ghcr): the instance boots with the
# prover prebuilt, sources ship as runtime inputs, and the receipts come
# back proven by the CANONICAL guest (id in CANONICAL_IMAGE_ID, baked by
# .github/workflows/bake-prover-image.yml). Multi-TU builds (--build, see
# docs/architecture.md) rsync the manifest's directory to the instance and
# wrap every receipt of the chain locally afterwards.
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
#   --build FILE      build.json manifest instead of a single source
#   --gpu NAME        GPU model filter            (default: RTX_4090)
#   --max-dph N       max $/hr                    (default: 0.60)
#   --image IMG       docker image                (default: ghcr.io/nikicat/tcc-verified-prover:latest)
#                     stock images (slow path, provisions + builds on boot):
#                     --image nvidia/cuda:12.6.3-devel-ubuntu24.04
#   --instance ID     reuse an existing instance (skip search/create)
#   --keep            don't destroy the instance afterwards
#   --skip-wrap       stop after fetching the succinct receipt(s)
#
# NOTE: vastai CLI output formats drift between versions; the jq paths below
# are for vastai 0.5.x.
set -euo pipefail
cd "$(dirname "$0")/.."

# ---------------------------------------------------------------- args ----
SRC=""
BUILD=""
GPU="RTX_4090"
MAX_DPH="0.60"
IMAGE="ghcr.io/nikicat/tcc-verified-prover:latest"
INSTANCE=""
KEEP=0
SKIP_WRAP=0
while [ $# -gt 0 ]; do
    case "$1" in
        --build) BUILD="$2"; shift 2 ;;
        --gpu) GPU="$2"; shift 2 ;;
        --max-dph) MAX_DPH="$2"; shift 2 ;;
        --image) IMAGE="$2"; shift 2 ;;
        --instance) INSTANCE="$2"; shift 2 ;;
        --keep) KEEP=1; shift ;;
        --skip-wrap) SKIP_WRAP=1; shift ;;
        *) SRC="$1"; shift ;;
    esac
done
[ -n "$SRC" ] || [ -n "$BUILD" ] || { echo "usage: $0 <source.c> | --build <build.json> [--gpu RTX_4090] [--max-dph 0.60] [--keep]"; exit 2; }
[ -z "$SRC" ] || [ -f "$SRC" ] || { echo "no such source: $SRC"; exit 2; }
[ -z "$BUILD" ] || [ -f "$BUILD" ] || { echo "no such build manifest: $BUILD"; exit 2; }

# ------------------------------------------------------------- logging ----
# every stage line goes, timestamped and unbuffered, to $PROGRESS_FILE —
# `tail -f` it from anywhere regardless of stdout buffering
T_START=$(date +%s)
PROGRESS_FILE="${PROGRESS_FILE:-/tmp/prove-remote-progress.log}"
: > "$PROGRESS_FILE"
log() {
    local msg="[prove-remote +$(($(date +%s) - T_START))s] $*"
    printf '\033[1;34m%s\033[0m\n' "$msg"
    printf '%s\n' "$msg" >> "$PROGRESS_FILE"
}

# --------------------------------------------------------- ssh helpers ----
# deterministic single-key auth: agents with many (wrong) keys trip hosts'
# MaxAuthTries ("Too many authentication failures"), and one auth attempt
# also exposes hosts where vast's key injection failed, immediately
SSH_KEY="${VAST_SSH_KEY:-$HOME/.ssh/id_ed25519}"
SSH_BASE=(-F /dev/null -o IdentityAgent=none -o IdentitiesOnly=yes -i "$SSH_KEY"
          -o StrictHostKeyChecking=accept-new -o ConnectTimeout=10)
SSH_HOST=""
SSH_PORT=""

# run a command on the instance: vssh 'cd /x && do-thing'
vssh() {
    ssh -p "$SSH_PORT" "${SSH_BASE[@]}" "root@$SSH_HOST" "$@"
}

# fetch remote files: vfetch <remote-path>... <local-dir>
# (remote globs are expanded by the remote shell)
vfetch() {
    local dest="${*: -1}"
    local srcs=("${@:1:$#-1}")
    scp -P "$SSH_PORT" "${SSH_BASE[@]}" "${srcs[@]/#/root@$SSH_HOST:}" "$dest"
}

# push a directory: vpush <local-dir>/ <remote-dir>/ [rsync-opts...]
vpush() {
    local src="$1" dst="$2"
    shift 2
    rsync -az --delete "$@" -e "ssh -p $SSH_PORT ${SSH_BASE[*]}" \
        "$src" "root@$SSH_HOST:$dst"
}

# candidate ssh endpoints for the instance, one host:port per line. The API
# may advertise several (the CLI's ssh-url resolution, proxy
# ssh_host:ssh_port, direct public_ipaddr + 22/tcp mapping) and hosts differ
# in which one actually routes.
ssh_endpoints() {
    local info="$1" url h p
    url=$(vastai ssh-url "$INSTANCE" 2>/dev/null || true)
    [[ "$url" =~ ^ssh://root@([^:]+):([0-9]+) ]] && echo "${BASH_REMATCH[1]}:${BASH_REMATCH[2]}"
    h=$(jq -r '.ssh_host // empty' <<<"$info")
    p=$(jq -r 'if .ssh_port then .ssh_port|tostring else empty end' <<<"$info")
    [ -n "$h" ] && [ -n "$p" ] && echo "$h:$p"
    h=$(jq -r '.public_ipaddr // empty' <<<"$info")
    p=$(jq -r '.ports["22/tcp"][0].HostPort // empty' <<<"$info" 2>/dev/null)
    [ -n "$h" ] && [ -n "$p" ] && echo "$h:$p"
    true
}

# probe candidates until one answers; sets SSH_HOST/SSH_PORT for vssh & co.
# Patience: weak hosts can take 15+ min to pull/extract a ~17 GB image;
# status_msg distinguishes slow ("Pulling 67%...") from dead.
ssh_wait() {
    local info cands c
    for i in $(seq 1 280); do
        info=$(vastai show instance "$INSTANCE" --raw)
        cands=$(ssh_endpoints "$info" | sort -u)
        for c in $cands; do
            SSH_HOST=${c%:*}
            SSH_PORT=${c#*:}
            vssh true 2>/dev/null && { log "ssh: root@$SSH_HOST:$SSH_PORT"; return 0; }
        done
        [ $((i % 12)) = 0 ] && log "still waiting for ssh (tried: ${cands//$'\n'/ }; status: $(jq -r '.status_msg // "-"' <<<"$info" | tr '\n' ' ' | head -c 90))"
        sleep 5
    done
    echo "ssh never came up (endpoints tried: ${cands//$'\n'/ }, last status: $(jq -r '.status_msg // "-"' <<<"$info"))"
    return 1
}

# --------------------------------------------- vast.ai instance lifecycle ----
CREATED=0

instance_create() {
    log "searching offers: $GPU, <= \$$MAX_DPH/hr, reliable, fast net..."
    local offer offer_id create_out
    offer=$(vastai search offers \
        "gpu_name=$GPU num_gpus=1 rentable=true reliability>0.99 cuda_vers>=12.6 inet_down>200 disk_space>40 dph<=$MAX_DPH" \
        -o dph --raw | jq '.[0]')
    [ "$offer" != "null" ] || { echo "no offers matched"; exit 1; }
    offer_id=$(jq -r '.id' <<<"$offer")
    log "cheapest offer: id=$offer_id $(jq -r '"\(.gpu_name) @ $\(.dph_total)/hr, \(.inet_down)Mbps down, reliability \(.reliability2 // .reliability)"' <<<"$offer")"

    log "creating instance (image: $IMAGE)..."
    create_out=$(vastai create instance "$offer_id" --image "$IMAGE" --disk 48 --ssh --direct --raw)
    INSTANCE=$(jq -r '.new_contract' <<<"$create_out" 2>/dev/null) || true
    [ -n "$INSTANCE" ] && [ "$INSTANCE" != "null" ] || { echo "create failed, raw output:"; echo "$create_out"; exit 1; }
    CREATED=1
    log "instance id: $INSTANCE"
}

instance_destroy() {
    if [ "$KEEP" = 0 ] && [ "$CREATED" = 1 ]; then
        log "destroying instance $INSTANCE"
        vastai destroy instance "$INSTANCE" || echo "WARNING: destroy failed - check https://cloud.vast.ai to avoid charges!"
    elif [ -n "$INSTANCE" ]; then
        log "keeping instance $INSTANCE (remember: it bills until destroyed)"
    fi
}

instance_wait_running() {
    log "waiting for instance to start (baked image pull ~3-5 min)..."
    local status="" info
    for i in $(seq 1 90); do
        info=$(vastai show instance "$INSTANCE" --raw)
        # API reports progress inconsistently: actual_status may stay null
        # while cur_state says running; trust either, the ssh probe decides.
        status=$(jq -r '.actual_status // .cur_state // empty' <<<"$info")
        [ "$status" = "running" ] && return 0
        [ $((i % 6)) = 0 ] && log "still waiting for instance (status: ${status:-none})..."
        sleep 10
    done
    echo "instance never reached running state: $(jq -c '{actual_status,cur_state,status_msg}' <<<"$info")"
    return 1
}

# ------------------------------------------------------- prover setup ----
sync_and_build() {
    vpush ./ /opt/tcc-verified/ \
        --exclude target --exclude out --exclude .git \
        --exclude examples/musl-hello/musl --exclude examples/musl-hello/build
    vssh "bash /opt/tcc-verified/cloud/provision.sh" | tail -1
}

remote_host() {
    vssh "cd /opt/tcc-verified && RISC0_PROVER=local RUST_LOG=info ./target/release/host $*" \
        2>&1 | tee -a "$PROGRESS_FILE"
}

# make sure the instance's prover exists and its guest matches the pin
ensure_canonical_guest() {
    if vssh '[ -x /opt/tcc-verified/target/release/host ]' 2>/dev/null; then
        log "prebaked image detected"
    else
        log "stock image: rsyncing project + provisioning (~35 min first boot)..."
        sync_and_build
    fi

    local remote_id
    remote_id=$(vssh 'cd /opt/tcc-verified && ./target/release/host --image-id')
    if [ -n "$EXPECTED_ID" ] && [ "$remote_id" != "$EXPECTED_ID" ]; then
        # stale baked image (e.g. guest changed, CI hasn't rebaked): rebuild
        # incrementally against the image's warm cargo target (~10-15 min) —
        # provision.sh skips the toolchain steps when they're already present
        log "remote guest $remote_id != expected $EXPECTED_ID; rebuilding from local sources..."
        sync_and_build
        remote_id=$(vssh 'cd /opt/tcc-verified && ./target/release/host --image-id')
        if [ "$remote_id" != "$EXPECTED_ID" ]; then
            echo "FATAL: remote IMAGE_ID $remote_id != expected $EXPECTED_ID even after rebuild"
            echo "(local sources out of sync with the pin? guest not reproducible on the image?)"
            exit 1
        fi
    fi
    log "image id: $remote_id"
}

# ------------------------------------------- local wrap + verification ----
PODMAN_DOCKER_HOST="${DOCKER_HOST:-unix:///run/user/$(id -u)/podman/podman.sock}"

wrap_local() { # wrap_local <succinct-receipt> <out-dir>
    DOCKER_HOST="$PODMAN_DOCKER_HOST" RISC0_PROVER=local \
        ./target/release/host --wrap "$1" --out-dir "$2"
}

# ------------------------------------------------------- proving flows ----
prove_build() {
    local bdir bname rdir remote_bdir output t0
    bdir=$(dirname "$BUILD")
    bname=$(basename "$BUILD")
    rdir="out/remote-$(basename "$bdir")"
    # manifests may reference repo files outside their dir (../../vendor/...),
    # so a repo-relative build tree lands at the same path inside the
    # instance's repo copy; absolute/outside trees fall back to /tmp
    case "$bdir" in
        /*) remote_bdir="/tmp/buildtree" ;;
        *) remote_bdir="/opt/tcc-verified/$bdir" ;;
    esac
    log "rsyncing build tree $bdir -> $remote_bdir (excluding scratch build/)..."
    vpush "$bdir/" "$remote_bdir/" --exclude build --exclude .git

    log "proving build $BUILD (succinct, GPU)..."
    t0=$(date +%s)
    remote_host "--build $remote_bdir/$bname --mode succinct --out-dir /tmp/out"
    log "remote GPU proving wall time: $(($(date +%s) - t0))s"

    output=$(jq -r '.output' "$BUILD")
    mkdir -p "$rdir"
    vfetch "/tmp/out/receipt.*.bin" /tmp/out/chain.json "/tmp/out/$output" "$rdir/"
    log "fetched: $(ls "$rdir" | wc -l) files (receipt chain + $output)"

    if [ "$SKIP_WRAP" = 0 ]; then
        log "wrapping every receipt of the chain to groth16 locally (podman)..."
        local r
        for r in "$rdir"/receipt.*.succinct.bin; do
            wrap_local "$r" "$rdir"
            rm "$r"
        done
        jq '.receipts |= map(sub("\\.succinct\\.bin$"; ".groth16.bin"))' \
            "$rdir/chain.json" >"$rdir/chain.json.tmp" && mv "$rdir/chain.json.tmp" "$rdir/chain.json"
    fi
    ./target/release/verifier --chain "$rdir/chain.json" "${VERIFY_ARGS[@]}"
    log "done. artifacts in $rdir/"
}

prove_single() {
    local rdir t0
    rdir="out/remote-$(basename "$SRC" .c)"

    log "proving $SRC (succinct, GPU)..."
    scp -P "$SSH_PORT" "${SSH_BASE[@]}" "$SRC" "root@$SSH_HOST:/tmp/input.c"
    t0=$(date +%s)
    remote_host "/tmp/input.c --mode succinct --out-dir /tmp/out"
    log "remote GPU proving wall time: $(($(date +%s) - t0))s"

    mkdir -p "$rdir"
    vfetch /tmp/out/receipt.succinct.bin /tmp/out/hello "$rdir/"
    log "fetched: $rdir/receipt.succinct.bin ($(stat -c%s "$rdir/receipt.succinct.bin") bytes) + binary"

    if [ "$SKIP_WRAP" = 0 ]; then
        log "wrapping to groth16 locally (podman)..."
        wrap_local "$rdir/receipt.succinct.bin" "$rdir"
        ./target/release/verifier "$rdir/receipt.groth16.bin" "$rdir/hello" "$SRC" "${VERIFY_ARGS[@]}"
    else
        ./target/release/verifier "$rdir/receipt.succinct.bin" "$rdir/hello" "$SRC" "${VERIFY_ARGS[@]}"
    fi
    log "done. artifacts in $rdir/"
}

# ---------------------------------------------------------------- main ----
log "progress log: $PROGRESS_FILE"

# canonical id: pinned in-repo by CI bake; fall back to local build's id
EXPECTED_ID=$(cat CANONICAL_IMAGE_ID 2>/dev/null || ./target/release/host --image-id 2>/dev/null || true)
VERIFY_ARGS=()
[ -n "$EXPECTED_ID" ] && VERIFY_ARGS=(--image-id "$EXPECTED_ID")

[ -n "$INSTANCE" ] || instance_create
trap instance_destroy EXIT

instance_wait_running
ssh_wait
# sanity: some hosts rent out containers without a working CUDA device
vssh 'nvidia-smi -L' >/dev/null 2>&1 || { echo "FATAL: no CUDA device visible on the rented host — destroy and re-rent"; exit 1; }

ensure_canonical_guest

if [ -n "$BUILD" ]; then
    prove_build
else
    prove_single
fi
