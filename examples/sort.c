/* sorting: recursive quicksort of 100,000 xorshift64-generated u64s,
 * self-checks sortedness and prints an FNV-1a hash of the result.
 * Freestanding. Output is deterministic; sorted=1 means correct. */

static long sys3(long n, long a, long b, long c)
{
    long ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "a"(n), "D"(a), "S"(b), "d"(c)
                         : "rcx", "r11", "memory");
    return ret;
}

static void print(const char *s, unsigned long len) { sys3(1, 1, (long)s, len); }

static void print_u64(unsigned long v)
{
    char buf[24];
    int i = 24;
    buf[--i] = '\n';
    do {
        buf[--i] = '0' + (v % 10);
        v /= 10;
    } while (v);
    print(buf + i, 24 - i);
}

#define N 100000
static unsigned long a[N];

static void quicksort(long lo, long hi)
{
    if (lo >= hi)
        return;
    unsigned long p = a[(lo + hi) / 2];
    long i = lo, j = hi;
    while (i <= j) {
        while (a[i] < p)
            i++;
        while (a[j] > p)
            j--;
        if (i <= j) {
            unsigned long t = a[i];
            a[i] = a[j];
            a[j] = t;
            i++;
            j--;
        }
    }
    quicksort(lo, j);
    quicksort(i, hi);
}

void _start(void)
{
    unsigned long x = 88172645463325252UL;
    for (long i = 0; i < N; i++) {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        a[i] = x;
    }
    quicksort(0, N - 1);

    unsigned long sorted = 1, h = 14695981039346656037UL;
    for (long i = 0; i < N; i++) {
        if (i && a[i - 1] > a[i])
            sorted = 0;
        h = (h ^ a[i]) * 1099511628211UL;
    }
    print("sorted=", 7);
    print_u64(sorted);
    print("fnv=", 4);
    print_u64(h);
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
