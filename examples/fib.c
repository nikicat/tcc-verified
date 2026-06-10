/* fibonacci: iterative u64, prints fib(90). Freestanding, no libc.
 * expected output: fib(90)=2880067194370816120 */

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

static unsigned long fib(int n)
{
    unsigned long a = 0, b = 1, t;
    while (n--) {
        t = a + b;
        a = b;
        b = t;
    }
    return a;
}

void _start(void)
{
    print("fib(90)=", 8);
    print_u64(fib(90));
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
