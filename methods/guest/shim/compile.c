/* zk_compile(): the single FFI entry point the Rust guest calls.
 * Drives libtcc: C source string in, linked x86-64 ELF executable out. */
#include <string.h>

#include "libtcc.h"

extern void zk_out_file(const unsigned char **p, unsigned long *n);

/* Compiler flags are part of the proof statement: they are baked into this
 * guest image and therefore pinned by the IMAGE_ID. */
static const char *const TCC_OPTIONS = "-static -nostdlib";

static char errbuf[1 << 16];
static unsigned long errlen;

static void err_cb(void *opaque, const char *msg)
{
    unsigned long n = strlen(msg);
    unsigned long room = sizeof errbuf - errlen;
    (void)opaque;
    if (n > room)
        n = room;
    memcpy(errbuf + errlen, msg, n);
    errlen += n;
    if (errlen < sizeof errbuf)
        errbuf[errlen++] = '\n';
}

int zk_compile(const char *src, const unsigned char **out,
               unsigned long *out_len, const char **err,
               unsigned long *err_len)
{
    TCCState *s;
    int rc = -1;

    *out = 0;
    *out_len = 0;
    s = tcc_new();
    if (!s) {
        *err = "tcc_new failed";
        *err_len = strlen(*err);
        return -1;
    }
    tcc_set_error_func(s, 0, err_cb);
    tcc_set_options(s, TCC_OPTIONS);
    tcc_set_output_type(s, TCC_OUTPUT_EXE);

    if (tcc_compile_string(s, src) != -1 &&
        tcc_output_file(s, "out.elf") != -1)
        rc = 0;

    tcc_delete(s);
    zk_out_file(out, out_len);
    *err = errbuf;
    *err_len = errlen;
    return rc;
}
