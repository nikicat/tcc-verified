/* Newlib OS layer for running TCC inside the RISC Zero zkVM guest.
 *
 * The guest has no kernel and no real filesystem. This file provides an
 * in-memory one: the Rust guest registers the job's input files
 * (zk_add_file) before driving TCC; TCC then open()s sources, headers and
 * object files by path, and writes its outputs (.o files, the linked ELF)
 * into a static arena, from which the Rust guest fetches them back
 * (zk_get_file). Plus the usual scaffolding: bump-allocator _sbrk for
 * newlib malloc, captured console, time() pinned to 0 => deterministic.
 */
#include <errno.h>
#include <fcntl.h>
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

/* ---------- in-memory filesystem ---------- */
#define MFS_MAX_FILES 4096
#define MFS_MAX_FDS 128
#define MFS_PATH_MAX 200
#define MFS_FD_BASE 4

struct mfile {
    char path[MFS_PATH_MAX];
    const unsigned char *ro; /* input data (Rust-owned), or NULL */
    unsigned char *rw;       /* output data in the arena, or NULL */
    size_t len;
    size_t cap; /* rw only */
    int deleted;
};
static struct mfile files[MFS_MAX_FILES];
static int n_files;

static unsigned char out_arena[32 << 20] __attribute__((aligned(16)));
static size_t arena_off;
static int cur_writer = -1; /* index of the file open for write, or -1 */

struct mfd {
    int used;
    int file;
    size_t pos;
    int writable;
};
static struct mfd fds[MFS_MAX_FDS];

/* Lexically normalize a path: drop leading '/' and "./", collapse "//",
 * resolve "..". TCC builds include paths by concatenation
 * ("src/include" + "/" + "../../include/errno.h"), so this is required
 * for memfs lookups to hit. */
static int mfs_norm(const char *in, char *out)
{
    const char *seg[64];
    int seglen[64];
    int n = 0;
    const char *p = in;

    while (*p) {
        while (*p == '/')
            p++;
        if (!*p)
            break;
        const char *start = p;
        while (*p && *p != '/')
            p++;
        int len = p - start;
        if (len == 1 && start[0] == '.')
            continue;
        if (len == 2 && start[0] == '.' && start[1] == '.') {
            if (n > 0)
                n--; /* above-root ".." sticks at root */
            continue;
        }
        if (n == 64)
            return -1;
        seg[n] = start;
        seglen[n] = len;
        n++;
    }
    char *o = out;
    for (int i = 0; i < n; i++) {
        if (o - out + seglen[i] + 2 > MFS_PATH_MAX)
            return -1;
        if (i)
            *o++ = '/';
        memcpy(o, seg[i], seglen[i]);
        o += seglen[i];
    }
    *o = 0;
    return 0;
}

static int mfs_find(const char *norm)
{
    for (int i = 0; i < n_files; i++)
        if (!files[i].deleted && !strcmp(files[i].path, norm))
            return i;
    return -1;
}

/* Register an input file (called from Rust before running ops; data stays
 * owned by Rust and must outlive the job). */
void zk_add_file(const char *path, const unsigned char *data, unsigned long len)
{
    char norm[MFS_PATH_MAX];
    if (mfs_norm(path, norm) != 0 || n_files == MFS_MAX_FILES)
        zk_rust_abort(102);
    int i = mfs_find(norm);
    if (i >= 0)
        zk_rust_abort(103); /* duplicate input path */
    struct mfile *f = &files[n_files++];
    strcpy(f->path, norm);
    f->ro = data;
    f->len = len;
}

/* Fetch a file (input or produced output) by path. Returns 0 on success. */
int zk_get_file(const char *path, const unsigned char **data, unsigned long *len)
{
    char norm[MFS_PATH_MAX];
    if (mfs_norm(path, norm) != 0)
        return -1;
    int i = mfs_find(norm);
    if (i < 0)
        return -1;
    *data = files[i].ro ? files[i].ro : files[i].rw;
    *len = files[i].len;
    return 0;
}

static int fd_alloc(int file, int writable)
{
    for (int i = 0; i < MFS_MAX_FDS; i++) {
        if (!fds[i].used) {
            fds[i].used = 1;
            fds[i].file = file;
            fds[i].pos = 0;
            fds[i].writable = writable;
            return MFS_FD_BASE + i;
        }
    }
    errno = EMFILE;
    return -1;
}

static struct mfd *fd_get(int fd)
{
    if (fd < MFS_FD_BASE || fd >= MFS_FD_BASE + MFS_MAX_FDS || !fds[fd - MFS_FD_BASE].used)
        return 0;
    return &fds[fd - MFS_FD_BASE];
}

int _open(const char *name, int flags, ...)
{
    char norm[MFS_PATH_MAX];
    if (mfs_norm(name, norm) != 0) {
        errno = ENAMETOOLONG;
        return -1;
    }
    if ((flags & O_ACCMODE) == O_RDONLY) {
        int i = mfs_find(norm);
        if (i < 0) {
            errno = ENOENT;
            return -1;
        }
        return fd_alloc(i, 0);
    }
    /* write: one writer at a time, fresh arena tail (O_TRUNC semantics) */
    if (cur_writer != -1) {
        errno = EACCES;
        return -1;
    }
    if (n_files == MFS_MAX_FILES) {
        errno = ENFILE;
        return -1;
    }
    int old = mfs_find(norm);
    if (old >= 0)
        files[old].deleted = 1;
    int i = n_files++;
    struct mfile *f = &files[i];
    strcpy(f->path, norm);
    f->ro = 0;
    f->rw = out_arena + arena_off;
    f->len = 0;
    f->cap = sizeof out_arena - arena_off;
    f->deleted = 0;
    int fd = fd_alloc(i, 1);
    if (fd >= 0)
        cur_writer = i;
    return fd;
}

int _write(int fd, const void *buf, size_t n)
{
    if (fd == 1 || fd == 2) {
        con_write(buf, n);
        return (int)n;
    }
    struct mfd *d = fd_get(fd);
    if (!d || !d->writable) {
        errno = EBADF;
        return -1;
    }
    struct mfile *f = &files[d->file];
    if (d->pos + n > f->cap) {
        errno = EFBIG;
        return -1;
    }
    memcpy(f->rw + d->pos, buf, n);
    d->pos += n;
    if (d->pos > f->len)
        f->len = d->pos;
    return (int)n;
}

int _read(int fd, void *buf, size_t n)
{
    struct mfd *d = fd_get(fd);
    if (!d)
        return 0; /* stdin: EOF */
    struct mfile *f = &files[d->file];
    const unsigned char *data = f->ro ? f->ro : f->rw;
    size_t avail = f->len > d->pos ? f->len - d->pos : 0;
    if (n > avail)
        n = avail;
    memcpy(buf, data + d->pos, n);
    d->pos += n;
    return (int)n;
}

off_t _lseek(int fd, off_t off, int whence)
{
    struct mfd *d = fd_get(fd);
    if (!d)
        return 0;
    struct mfile *f = &files[d->file];
    long np = whence == 0 ? (long)off          /* SEEK_SET */
            : whence == 1 ? (long)d->pos + off /* SEEK_CUR */
                          : (long)f->len + off; /* SEEK_END */
    if (np < 0 || (d->writable && (size_t)np > f->cap) ||
        (!d->writable && (size_t)np > f->len)) {
        errno = EINVAL;
        return (off_t)-1;
    }
    if (d->writable && (size_t)np > f->len)
        f->len = (size_t)np; /* zero gap is already zero (bss arena) */
    d->pos = (size_t)np;
    return (off_t)np;
}

int _close(int fd)
{
    struct mfd *d = fd_get(fd);
    if (!d)
        return 0;
    if (d->writable && d->file == cur_writer) {
        /* finalize: claim the written bytes from the arena */
        arena_off += files[d->file].len;
        arena_off = (arena_off + 15) & ~(size_t)15;
        files[d->file].cap = files[d->file].len;
        cur_writer = -1;
    }
    d->used = 0;
    return 0;
}

int _fstat(int fd, struct stat *st)
{
    memset(st, 0, sizeof *st);
    struct mfd *d = fd_get(fd);
    st->st_mode = d ? S_IFREG : S_IFCHR;
    st->st_size = d ? (off_t)files[d->file].len : 0;
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

int _unlink(const char *p)
{
    char norm[MFS_PATH_MAX];
    if (mfs_norm(p, norm) == 0) {
        int i = mfs_find(norm);
        if (i >= 0)
            files[i].deleted = 1;
    }
    return 0;
}

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
