# Operations — cloud proving, CI, and the sharp edges

## Cloud proving runbook (vast.ai)

```sh
vastai set api-key <KEY>            # + ssh key attached to account
cloud/prove-remote.sh examples/foo.c            # full cycle, ~8 min cold
cloud/prove-remote.sh foo.c --instance <ID> --keep   # reuse a warm box
```

- Default image: `ghcr.io/nikicat/tcc-verified-prover:latest` (public).
  Instances boot it prebuilt; the source ships by scp as a runtime input.
- The script destroys instances it created (EXIT trap). It does NOT
  destroy `--instance` boxes — destroy manually: `vastai destroy instance
  <ID>`. **Check `vastai show instances` after any aborted session.**
- Economics: 4090 ≈ $0.30–0.45/hr; GPU proving ≈ 12 s + 1.55 s/segment;
  parking a box costs ~$9/day — destroy, recreate in ~4 min.
- **Exact spend**: `vastai show invoices --only_charges -s <date>` gives
  per-instance GPU and storage line items — trust it over mental
  arithmetic. Storage bills for the requested `--disk` for the whole
  instance lifetime (including boot/pull), and failed-but-waiting
  instances bill GPU time too: the 2026-06-11 live-debug session cost
  ~$0.60 total, of which ~$0.43 was four failed/aborted rentals and
  $0.174 the successful musl-hello run. Some hosts also bill bandwidth
  per GB (`inet_down_cost`, $/GB, up to ~100× across hosts) — the offer
  query caps it at $5/TB so image pulls stay ~free.
- Reliability floor is 0.99 in the offer query for a reason: a 0.983 host
  took the rental and never started (no logs, no ssh). Failed hosts cost
  cents — destroy and re-rent.
- **Probe every advertised ssh endpoint.** Three rentals in a row ran the
  container fine while the advertised proxy `ssh_host:ssh_port` never
  answered; the direct `public_ipaddr` + 22/tcp mapping worked
  immediately. `ssh_wait` now tries the CLI's `ssh-url`, the proxy pair,
  and the direct mapping each round. Also: after pulling the image, vast
  builds a derived overlay (its ssh/tooling injection shows up as
  buildkit `#N` steps in `status_msg`) — boot takes minutes longer than
  the pull itself, which `status_msg` logging makes visible.
- vastai CLI (0.5.x) quirks: some endpoints return non-JSON errors
  (`create failed, raw output:` handling exists); `actual_status` can stay
  null while `cur_state=running`; `show user` is broken in 0.5.0.

## CI bake (`bake-prover-image.yml`)

Push to main touching `cloud/Dockerfile`, `Cargo.lock`,
`rust-toolchain.toml`, or the workflow triggers a bake to ghcr.

- ~15 min warm (cargo-chef cook layer cached in the `:buildcache`
  registry ref), ~60 min cold (dependency/toolchain changes).
- The `guest-check` stage builds the guest in ~10 min in parallel with the
  cook and **fails the whole build fast** — without it, an early failure
  hides behind cargo draining hour-long nvcc jobs (cargo never kills
  in-flight jobs; `timeout-minutes: 150` is the backstop).
- The final step extracts the image's IMAGE_ID (libcuda stub trick — the
  host binary links libcuda even for `--image-id`; a stub .so generated
  from its undefined `cu*` symbols satisfies the BIND_NOW linker) and
  **fails on drift from `CANONICAL_IMAGE_ID`**. On an intentional compiler
  change: update the pin in the same PR.

## Multi-TU builds

- `prove-remote.sh --build examples/musl-hello/build.json` rsyncs the
  manifest's directory (pinned musl checkout included) to the instance —
  at its repo-relative path, so `../../vendor/...` references resolve
  against the image's repo copy — runs the batched jobs + link there,
  fetches the receipt chain, wraps *every* receipt to groth16 locally,
  and verifies `--chain`. Measured 2026-06-11 (see measurements.md):
  740 s GPU wall for the 401-segment chain, ~29 min end-to-end, ~12¢.
- Fan-out across instances: `host --build m.json --only N` on each box
  (receipts + objects land in the shared out dir), then `--only link`
  collects whatever `receipt.job*.bin` it finds. The objects move between
  boxes as plain files; the chain check catches any mixup by hash.
- Guest memfs limits (static, in `shim/sys.c`): 4096 files, 128 open fds,
  200-char normalized paths, 32 MB output arena, 64 MB C heap. A full-musl
  build (~1262 TUs in one job) would hit none of them per-batch, but keep
  batches ≤ a few hundred TUs so journals stay small.
- musl TUs that TCC cannot compile (37 of 1262, all x87 asm: `x`/`t`
  register constraints, `expl.s` syntax): irrelevant for musl-hello's
  closure, blocks "all of musl" — see roadmap.

## Sharp edges encountered (so you don't re-cut yourself)

1. **CUDA arch**: risc0-build-kernel passes `-arch=native` unless
   `NVCC_APPEND_FLAGS` is set; GPU-less builders must pin
   (`-gencode arch=compute_89,...`). Renting non-Ada GPUs older than sm_89
   requires a rebake with different arch flags.
2. **CUDA build deps**: the cuda feature chain needs `protobuf-compiler`
   (circom-witnesscalc) and `clang/libclang-dev` (bindgen) — CPU-only
   builds never exercise these.
3. **Vendoring vs nested .gitignore**: tinycc's own `.gitignore` shadows
   `conftest.c` etc.; `git add -A` silently skips them. Local builds work
   (file on disk), fresh checkouts break. Always `git add -f vendor/`.
4. **Guest reproducibility**: only inside the canonical container
   (identical paths). Local-vs-anywhere IMAGE_ID mismatch is expected;
   that's why the pin comes from the bake. assert/__FILE__ strings and
   cargo registry paths are the leak vectors.
5. **newlib ↔ Rust link conflicts**: rlib objects load eagerly; newlib
   members defining the same symbols (mem*, libm) must be deleted from a
   filtered libc.a (guest build.rs step 5).
6. **groth16 wrap needs a docker CLI**: rootless podman works
   (`DOCKER_HOST=unix://$XDG_RUNTIME_DIR/podman/podman.sock`,
   `systemctl --user start podman.socket`). Container-based GPU rentals
   can't docker-in-docker — hence wrap-locally in the driver. The rzup
   `risc0-groth16` component may offer a container-free path (untested).
7. **Default risc0 features**: `prove` is NOT in default features;
   without it `default_prover()` delegates to the external r0vm server.
   Host enables it explicitly; `RISC0_PROVER=local` forces in-process.
8. **risc0 serde costs a word per byte** on `Vec<u8>` — never `env::read`
   bulk data. Jobs cross host→guest as one length-prefixed `write_slice`
   of packed u32 words (`env::read_frame` would do exactly this but is
   feature-gated unstable in 3.0.5).
9. **The 2026-06-11 guest change (multi-TU, journal v2) moved IMAGE_ID**:
   the pin in `CANONICAL_IMAGE_ID` must be updated from the next CI bake
   (or a local `docker build --target guest-check` + id extraction) before
   the bake workflow goes green again. Old receipts keep verifying against
   the old pin; keep it listed for historical attestations.
