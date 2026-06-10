/* Newlib OS layer for running TCC inside the RISC Zero zkVM guest.
 *
 * The guest has no kernel and no filesystem. TCC, in our configuration
 * (tcc_compile_string + tcc_output_file, -static -nostdlib), touches the
 * OS only through:
 *   - malloc/free (newlib malloc -> _sbrk)
 *   - the output ELF: unlink(), open(), fdopen(), fwrite/fseek, fclose
 *   - stderr diagnostics
 *   - time() for __DATE__/__TIME__ (pinned to epoch 0 => deterministic)
 * Everything below backs those with static in-guest memory.
 */
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>

extern void zk_rust_abort(int code) __attribute__((noreturn));

/* ---------- captured console (fd 1/2) ---------- */
static char con_buf[1 << 16];
static size_t con_len;

void zk_console(const char **p, unsigned long *n)
{
    *p = con_buf;
    *n = con_len;
}

static void con_write(const char *b, size_t n)
{
    size_t room = sizeof con_buf - con_len;
    if (n > room)
        n = room;
    memcpy(con_buf + con_len, b, n);
    con_len += n;
}

/* ---------- C heap (newlib malloc backend) ---------- */
static char heap[64 << 20] __attribute__((aligned(16)));
static size_t brk_off;

void *_sbrk(ptrdiff_t incr)
{
    if (incr < 0 || brk_off + (size_t)incr > sizeof heap) {
        errno = ENOMEM;
        return (void *)-1;
    }
    void *p = heap + brk_off;
    brk_off += (size_t)incr;
    return p;
}

/* ---------- in-memory output file ---------- */
#define OUT_FD 3
static unsigned char out_store[16 << 20];
static size_t out_len, out_pos;

void zk_out_file(const unsigned char **p, unsigned long *n)
{
    *p = out_store;
    *n = out_len;
}

int _open(const char *name, int flags, ...)
{
    (void)name;
    (void)flags;
    out_len = out_pos = 0; /* O_TRUNC semantics; single output file */
    return OUT_FD;
}

int _write(int fd, const void *buf, size_t n)
{
    if (fd == 1 || fd == 2) {
        con_write(buf, n);
        return (int)n;
    }
    if (fd == OUT_FD) {
        if (out_pos + n > sizeof out_store) {
            errno = EFBIG;
            return -1;
        }
        memcpy(out_store + out_pos, buf, n);
        out_pos += n;
        if (out_pos > out_len)
            out_len = out_pos;
        return (int)n;
    }
    errno = EBADF;
    return -1;
}

off_t _lseek(int fd, off_t off, int whence)
{
    long np;
    if (fd != OUT_FD)
        return 0;
    np = whence == 0 ? (long)off            /* SEEK_SET */
       : whence == 1 ? (long)out_pos + off  /* SEEK_CUR */
                     : (long)out_len + off; /* SEEK_END */
    if (np < 0 || (size_t)np > sizeof out_store) {
        errno = EINVAL;
        return (off_t)-1;
    }
    if ((size_t)np > out_len)
        out_len = (size_t)np; /* zero gap is already zero (bss) */
    out_pos = (size_t)np;
    return (off_t)np;
}

int _read(int fd, void *buf, size_t n)
{
    if (fd == OUT_FD) {
        size_t avail = out_len > out_pos ? out_len - out_pos : 0;
        if (n > avail)
            n = avail;
        memcpy(buf, out_store + out_pos, n);
        out_pos += n;
        return (int)n;
    }
    return 0;
}

int _close(int fd)
{
    (void)fd;
    return 0;
}

int _fstat(int fd, struct stat *st)
{
    memset(st, 0, sizeof *st);
    st->st_mode = fd <= 2 ? S_IFCHR : S_IFREG;
    st->st_size = fd == OUT_FD ? (off_t)out_len : 0;
    st->st_blksize = 1024;
    return 0;
}

int _stat(const char *path, struct stat *st)
{
    (void)path;
    (void)st;
    errno = ENOENT;
    return -1;
}

int _isatty(int fd) { return fd <= 2; }
int _unlink(const char *p) { (void)p; return 0; }
int _getpid(void) { return 1; }
int _kill(int pid, int sig)
{
    (void)pid;
    zk_rust_abort(128 + sig);
}
void _exit(int code) { zk_rust_abort(code); }

int _gettimeofday(struct timeval *tv, void *tz)
{
    (void)tz;
    if (tv)
        memset(tv, 0, sizeof *tv);
    return 0;
}

clock_t _times(struct tms *t)
{
    if (t)
        memset(t, 0, sizeof *t);
    return 0;
}

/* newlib getenv() walks this; empty environment */
char **environ;

/* TCC's include-path normalization; no filesystem, so identity/root */
char *getcwd(char *buf, size_t size)
{
    if (size < 2) {
        errno = ERANGE;
        return 0;
    }
    buf[0] = '/';
    buf[1] = 0;
    return buf;
}

char *realpath(const char *path, char *resolved)
{
    size_t n = strlen(path);
    if (n >= 4096)
        n = 4095;
    memcpy(resolved, path, n);
    resolved[n] = 0;
    return resolved;
}

/* newlib rv32 has no ldexpl (long double = binary128). Multiplying by
 * powers of two is exact, so exponentiation-by-squaring is a correct
 * implementation (modulo subnormal rounding, irrelevant for TCC's
 * constant folding). */
long double ldexpl(long double x, int n)
{
    long double f = 2.0L;
    int neg = n < 0;
    unsigned m = neg ? (unsigned)-(long)n : (unsigned)n;
    while (m) {
        if (m & 1)
            x = neg ? x / f : x * f;
        f *= f;
        m >>= 1;
    }
    return x;
}
