/* Freestanding greeter, compiled as its own translation unit. */
#include "greet.h"

long sys3(long n, long a, long b, long c)
{
    long ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "a"(n), "D"(a), "S"(b), "d"(c)
                         : "rcx", "r11", "memory");
    return ret;
}

void greet(void)
{
    static const char msg[] = "hello from two proven TUs\n";
    sys3(1, 1, (long)msg, sizeof msg - 1); /* write(STDOUT, ...) */
}
