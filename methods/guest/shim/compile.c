/* The FFI entry points the Rust guest calls to drive libtcc, one fresh
 * TCCState per op:
 *   zk_compile_obj(): one translation unit -> ELF object file (.o)
 *   zk_link():        object files -> linked static x86-64 executable
 *
 * Compiler flags are part of the proof statement: they are baked into this
 * guest image and therefore pinned by the IMAGE_ID. Include paths and
 * preprocessor defines are job inputs — they only steer resolution within
 * the committed input tree and are themselves committed to the journal. */
#include <string.h>

#include "libtcc.h"

static const char *const COMPILE_OPTIONS = "-nostdlib";
static const char *const LINK_OPTIONS = "-static -nostdlib";

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

static TCCState *new_state(const char *options)
{
    TCCState *s = tcc_new();
    if (!s)
        return 0;
    tcc_set_error_func(s, 0, err_cb);
    tcc_set_options(s, options);
    return s;
}

int zk_compile_obj(const char *src, const char *out,
                   const char *const *inc_dirs, unsigned n_inc,
                   const char *const *def_names, const char *const *def_values,
                   unsigned n_def, const char **err, unsigned long *err_len)
{
    TCCState *s;
    unsigned i;
    int rc = -1;

    errlen = 0;
    s = new_state(COMPILE_OPTIONS);
    if (!s)
        goto out;
    for (i = 0; i < n_inc; i++)
        if (tcc_add_include_path(s, inc_dirs[i]) == -1)
            goto del;
    for (i = 0; i < n_def; i++)
        tcc_define_symbol(s, def_names[i], def_values[i]);
    tcc_set_output_type(s, TCC_OUTPUT_OBJ);

    if (tcc_add_file(s, src) != -1 && tcc_output_file(s, out) != -1)
        rc = 0;
del:
    tcc_delete(s);
out:
    *err = errbuf;
    *err_len = errlen;
    return rc;
}

int zk_link(const char *const *objs, unsigned n_objs, const char *out,
            const char **err, unsigned long *err_len)
{
    TCCState *s;
    unsigned i;
    int rc = -1;

    errlen = 0;
    s = new_state(LINK_OPTIONS);
    if (!s)
        goto out;
    tcc_set_output_type(s, TCC_OUTPUT_EXE);
    for (i = 0; i < n_objs; i++)
        if (tcc_add_file(s, objs[i]) == -1)
            goto del;
    if (tcc_output_file(s, out) != -1)
        rc = 0;
del:
    tcc_delete(s);
out:
    *err = errbuf;
    *err_len = errlen;
    return rc;
}
