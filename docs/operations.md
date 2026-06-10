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
- Reliability floor is 0.99 in the offer query for a reason: a 0.983 host
  took the rental and never started (no logs, no ssh). Failed hosts cost
  cents — destroy and re-rent.
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
