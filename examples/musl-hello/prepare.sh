#!/usr/bin/env bash
# Prepare the musl-hello build manifest:
#   1. clone musl at a pinned commit (provenance links to musl's own git)
#   2. generate musl's derived headers (alltypes.h, syscall.h, version.h)
#   3. build a *native* tcc from the vendored sources (the same compiler
#      the guest embeds) for fast out-of-zkVM closure computation
#   4. compile all of musl with it, mirroring the guest's flags exactly
#   5. compute the transitive symbol closure of hello.c + crt1.c, link and
#      run the result natively as a smoke test
#   6. emit build.json: the TU list + the union of headers they actually
#      include (from tcc -MD dependency output)
#
# The closure/native-link here is purely advisory: the proven build is what
# `host --build build.json` executes inside the zkVM.
set -euo pipefail
cd "$(dirname "$0")"

MUSL_TAG=v1.2.5
MUSL_COMMIT=0784374d561435f7c787a555aeab8ede699ed298
MUSL_URL=https://git.musl-libc.org/git/musl

# ---------- 1. pinned musl ----------
if [ ! -d musl ]; then
    git clone --depth 1 --branch "$MUSL_TAG" "$MUSL_URL" musl
fi
[ "$(git -C musl rev-parse HEAD)" = "$MUSL_COMMIT" ] || {
    echo "FATAL: musl checkout is not at pinned commit $MUSL_COMMIT" >&2
    exit 1
}

# ---------- 2. generated headers ----------
if [ ! -f musl/obj/include/bits/alltypes.h ]; then
    (cd musl && CC="${CC:-gcc}" ./configure --target=x86_64 >/dev/null &&
        make obj/include/bits/alltypes.h obj/include/bits/syscall.h \
            obj/src/internal/version.h >/dev/null)
fi

# ---------- 3. native tcc (vendored sources) ----------
TCC=build/tcc-src/tcc
if [ ! -x "$TCC" ]; then
    mkdir -p build
    rm -rf build/tcc-src
    cp -r ../../vendor/tinycc build/tcc-src
    (cd build/tcc-src && ./configure >/dev/null && make -j"$(nproc)" tcc >/dev/null)
fi

# ---------- 4. compile all of musl ----------
# Same include order and defines as musl's own Makefile; -nostdinc mirrors
# the guest, where no compiler-provided headers exist at all.
INC=(-Iarch/x86_64 -Iarch/generic -Iobj/src/internal -Isrc/include
    -Isrc/internal -Iobj/include -Iinclude)
DEFS=(-D_XOPEN_SOURCE=700)
OBJ=$PWD/build/obj
DEP=$PWD/build/dep
rm -rf "$OBJ" "$DEP"
mkdir -p "$OBJ" "$DEP"
TCCABS=$PWD/$TCC

# musl's source selection rule: src/<area>/x86_64/<name>.{c,s} replaces
# src/<area>/<name>.c. complex/ needs _Complex (tcc lacks it) and ldso/ is
# the dynamic linker: both irrelevant for a static hello.
mapfile -t SRCS < <(
    cd musl
    for area in src/*/; do
        area=${area#src/}
        area=${area%/}
        [ "$area" = complex ] || [ "$area" = ldso ] && continue
        for f in src/$area/*.c; do
            [ -e "$f" ] || continue
            stem=$(basename "$f" .c)
            if [ -e "src/$area/x86_64/$stem.s" ]; then
                echo "src/$area/x86_64/$stem.s"
            elif [ -e "src/$area/x86_64/$stem.c" ]; then
                echo "src/$area/x86_64/$stem.c"
            else
                echo "$f"
            fi
        done
        for f in src/$area/x86_64/*.s; do
            [ -e "$f" ] || continue
            stem=$(basename "$f" .s)
            [ -e "src/$area/$stem.c" ] || echo "$f"
        done
    done | sort -u
)

echo "compiling ${#SRCS[@]} musl TUs with native tcc..."
: >build/compile-failures.txt
(
    cd musl
    for f in "${SRCS[@]}"; do
        mkdir -p "$OBJ/$(dirname "$f")" "$DEP/$(dirname "$f")"
        if ! "$TCCABS" -c -nostdinc "${INC[@]}" "${DEFS[@]}" -MD -MF "$DEP/$f.d" \
            "$f" -o "$OBJ/$f.o" 2>>../build/compile-failures.txt; then
            echo "$f" >>../build/compile-failures.txt
        fi
    done
)
fails=$(grep -c '^src/' build/compile-failures.txt || true)
echo "  compiled; $fails TU(s) failed (ok unless needed by the closure)"

# crt1 + hello
mkdir -p "$OBJ/crt" "$DEP/crt"
(cd musl && "$TCCABS" -c -nostdinc "${INC[@]}" "${DEFS[@]}" -DCRT \
    -MD -MF "$DEP/crt/crt1.c.d" crt/crt1.c -o "$OBJ/crt/crt1.c.o")
"$TCCABS" -c -nostdinc "${INC[@]/-I/-Imusl/}" "${DEFS[@]}" \
    -MD -MF "$DEP/hello.c.d" hello.c -o "$OBJ/hello.c.o"

# TCC's compiler runtime (vendored): -nostdlib excludes libtcc1.a, but TCC
# emits calls to its helpers (long double conversions, x86-64 va_arg, ...).
# They are ordinary C TUs from the pinned vendor tree, so they join the
# closure pool like any musl file — under their real repo paths, so git
# linkage resolves them against tcc-verified itself.
TCCRT=vendor/tinycc/lib
mkdir -p "$OBJ/$TCCRT" "$DEP/$TCCRT"
for f in libtcc1.c va_list.c builtin.c stdatomic.c alloca.S; do
    "$TCCABS" -c -nostdinc "${INC[@]/-I/-Imusl/}" "${DEFS[@]}" \
        -MD -MF "$DEP/$TCCRT/$f.d" "../../$TCCRT/$f" \
        -o "$OBJ/$TCCRT/$f.o"
done

# ---------- 5. symbol closure ----------
# strong undefined (nm 'U') must be satisfied; weak undefined ('w'/'v')
# resolve to 0 in a static link, exactly musl's mechanism for optional
# subsystems.
defined() { nm -g --defined-only "$1" 2>/dev/null | awk '{print $NF}'; }
undefined() { nm -g -u "$1" 2>/dev/null | awk '$1 == "U" {print $2}'; }

declare -A SYM2OBJ # symbol -> first defining object (musl first, then tcc runtime)
while read -r o; do
    while read -r s; do
        [ -n "${SYM2OBJ[$s]:-}" ] || SYM2OBJ[$s]=$o
    done < <(defined "$o")
done < <(find "$OBJ/src" "$OBJ/vendor" -name '*.o' | sort)

declare -A HAVE PICKED
worklist=("$OBJ/hello.c.o" "$OBJ/crt/crt1.c.o")
PICKED[$OBJ/hello.c.o]=1
PICKED[$OBJ/crt/crt1.c.o]=1
while [ ${#worklist[@]} -gt 0 ]; do
    cur=("${worklist[@]}")
    worklist=()
    for o in "${cur[@]}"; do
        while read -r s; do HAVE[$s]=1; done < <(defined "$o")
    done
    for o in "${cur[@]}"; do
        while read -r s; do
            [ -n "${HAVE[$s]:-}" ] && continue
            prov=${SYM2OBJ[$s]:-}
            if [ -z "$prov" ]; then
                echo "FATAL: no musl object defines '$s' (needed by $o)" >&2
                exit 1
            fi
            if [ -z "${PICKED[$prov]:-}" ]; then
                PICKED[$prov]=1
                worklist+=("$prov")
            fi
        done < <(undefined "$o")
    done
done

# objects back to musl-relative source paths
TUS=()
for o in "${!PICKED[@]}"; do
    t=${o#"$OBJ/"}
    t=${t%.o}
    [ "$t" = hello.c ] || [ "$t" = crt/crt1.c ] && continue
    TUS+=("$t")
done
mapfile -t TUS < <(printf '%s\n' "${TUS[@]}" | sort)
echo "closure: ${#TUS[@]} musl TUs + crt1 + hello"

# native link + smoke test with the same object set
"$TCCABS" -static -nostdlib "$OBJ/crt/crt1.c.o" "$OBJ/hello.c.o" \
    $(printf "$OBJ/%s.o " "${TUS[@]}") -o build/hello-native
out=$(./build/hello-native)
[ "$out" = "hello, musl 42" ] || {
    echo "FATAL: native smoke test printed: $out" >&2
    exit 1
}
echo "native smoke test  : \"$out\" ✓"

# ---------- 6. emit build.json ----------
# headers = union of tcc -MD deps of the selected TUs (minus the sources)
{
    for t in "${TUS[@]}"; do cat "$DEP/$t.d"; done
    cat "$DEP/crt/crt1.c.d" "$DEP/hello.c.d"
} | tr ' \\' '\n\n' | grep -v ':$' | grep '\.h$' | sed 's|^musl/||' | sort -u \
    >build/headers.txt
printf '%s\n' "${TUS[@]}" >build/tus.txt

python3 - <<'EOF'
import json
import posixpath

# tcc -MD records paths as opened (e.g. src/include/../../include/stdio.h);
# normalize so each header appears exactly once, under its repo path
headers = sorted(
    {posixpath.normpath(l.strip()) for l in open("build/headers.txt") if l.strip()}
)
tus = [l.strip() for l in open("build/tus.txt") if l.strip()]

spec = {
    "files": [{"path": h, "from": f"musl/{h}"} for h in headers],
    "include_dirs": [
        "arch/x86_64", "arch/generic", "obj/src/internal", "src/include",
        "src/internal", "obj/include", "include",
    ],
    "defines": [["_XOPEN_SOURCE", "700"]],
    "tus": [{"src": "hello.c"}]
    + [{"src": "crt/crt1.c", "from": "musl/crt/crt1.c", "defines": [["CRT", None]]}]
    + [
        {"src": t, "from": f"../../{t}"}
        if t.startswith("vendor/")
        else {"src": t, "from": f"musl/{t}"}
        for t in tus
    ],
    "batch_size": 16,
    "output": "hello",
}
json.dump(spec, open("build.json", "w"), indent=2)
print(f"build.json: {len(spec['tus'])} TUs, {len(spec['files'])} shared files")
EOF
