/* prime finding: sieve of Eratosthenes up to 1,000,000. Freestanding.
 * expected output: count=78498  sum=37550402023 */

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

#define N 1000000
static unsigned char composite[N + 1];

void _start(void)
{
    unsigned long count = 0, sum = 0;
    for (unsigned long i = 2; i * i <= N; i++)
        if (!composite[i])
            for (unsigned long j = i * i; j <= N; j += i)
                composite[j] = 1;
    for (unsigned long i = 2; i <= N; i++)
        if (!composite[i]) {
            count++;
            sum += i;
        }
    print("count=", 6);
    print_u64(count);
    print("sum=", 4);
    print_u64(sum);
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
