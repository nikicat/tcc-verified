//! Embeds the repo's pinned canonical compiler id into the verifier binary,
//! so a built verifier carries its trust root (override with --image-id).

fn main() {
    let pin = std::fs::read_to_string("../CANONICAL_IMAGE_ID")
        .map(|s| s.trim().to_string())
        .unwrap_or_default();
    println!("cargo:rustc-env=CANONICAL_IMAGE_ID_HEX={pin}");
    println!("cargo:rerun-if-changed=../CANONICAL_IMAGE_ID");
}
