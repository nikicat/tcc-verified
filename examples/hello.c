/* Freestanding hello world for x86-64 Linux: no libc, no crt.
 * Compiled by TCC inside the zkVM with: -static -nostdlib */

static long sys3(long n, long a, long b, long c)
{
    long ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "a"(n), "D"(a), "S"(b), "d"(c)
                         : "rcx", "r11", "memory");
    return ret;
}

void _start(void)
{
    static const char msg[] = "hello world\n";
    sys3(1, 1, (long)msg, sizeof msg - 1); /* write(STDOUT, msg, 12) */
    sys3(60, 0, 0, 0);                     /* exit_group(0) */
    for (;;)
        ;
}
