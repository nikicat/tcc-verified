/* generated benchmark: 55 functions, freestanding (no libc) */
static long sys3(long n, long a, long b, long c)
{
    long ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "a"(n), "D"(a), "S"(b), "d"(c)
                         : "rcx", "r11", "memory");
    return ret;
}

static long f0(long x) { return ((x * 3 + 0) ^ (x >> 1)) - (x << 1); }
static long f1(long x) { return ((x * 5 + 1) ^ (x >> 2)) - (x << 1); }
static long f2(long x) { return ((x * 7 + 2) ^ (x >> 3)) - (x << 1); }
static long f3(long x) { return ((x * 9 + 3) ^ (x >> 4)) - (x << 1); }
static long f4(long x) { return ((x * 11 + 4) ^ (x >> 5)) - (x << 1); }
static long f5(long x) { return ((x * 3 + 5) ^ (x >> 6)) - (x << 1); }
static long f6(long x) { return ((x * 5 + 6) ^ (x >> 7)) - (x << 1); }
static long f7(long x) { return ((x * 7 + 7) ^ (x >> 1)) - (x << 1); }
static long f8(long x) { return ((x * 9 + 8) ^ (x >> 2)) - (x << 1); }
static long f9(long x) { return ((x * 11 + 9) ^ (x >> 3)) - (x << 1); }
static long f10(long x) { return ((x * 3 + 10) ^ (x >> 4)) - (x << 1); }
static long f11(long x) { return ((x * 5 + 11) ^ (x >> 5)) - (x << 1); }
static long f12(long x) { return ((x * 7 + 12) ^ (x >> 6)) - (x << 1); }
static long f13(long x) { return ((x * 9 + 13) ^ (x >> 7)) - (x << 1); }
static long f14(long x) { return ((x * 11 + 14) ^ (x >> 1)) - (x << 1); }
static long f15(long x) { return ((x * 3 + 15) ^ (x >> 2)) - (x << 1); }
static long f16(long x) { return ((x * 5 + 16) ^ (x >> 3)) - (x << 1); }
static long f17(long x) { return ((x * 7 + 17) ^ (x >> 4)) - (x << 1); }
static long f18(long x) { return ((x * 9 + 18) ^ (x >> 5)) - (x << 1); }
static long f19(long x) { return ((x * 11 + 19) ^ (x >> 6)) - (x << 1); }
static long f20(long x) { return ((x * 3 + 20) ^ (x >> 7)) - (x << 1); }
static long f21(long x) { return ((x * 5 + 21) ^ (x >> 1)) - (x << 1); }
static long f22(long x) { return ((x * 7 + 22) ^ (x >> 2)) - (x << 1); }
static long f23(long x) { return ((x * 9 + 23) ^ (x >> 3)) - (x << 1); }
static long f24(long x) { return ((x * 11 + 24) ^ (x >> 4)) - (x << 1); }
static long f25(long x) { return ((x * 3 + 25) ^ (x >> 5)) - (x << 1); }
static long f26(long x) { return ((x * 5 + 26) ^ (x >> 6)) - (x << 1); }
static long f27(long x) { return ((x * 7 + 27) ^ (x >> 7)) - (x << 1); }
static long f28(long x) { return ((x * 9 + 28) ^ (x >> 1)) - (x << 1); }
static long f29(long x) { return ((x * 11 + 29) ^ (x >> 2)) - (x << 1); }
static long f30(long x) { return ((x * 3 + 30) ^ (x >> 3)) - (x << 1); }
static long f31(long x) { return ((x * 5 + 31) ^ (x >> 4)) - (x << 1); }
static long f32(long x) { return ((x * 7 + 32) ^ (x >> 5)) - (x << 1); }
static long f33(long x) { return ((x * 9 + 33) ^ (x >> 6)) - (x << 1); }
static long f34(long x) { return ((x * 11 + 34) ^ (x >> 7)) - (x << 1); }
static long f35(long x) { return ((x * 3 + 35) ^ (x >> 1)) - (x << 1); }
static long f36(long x) { return ((x * 5 + 36) ^ (x >> 2)) - (x << 1); }
static long f37(long x) { return ((x * 7 + 37) ^ (x >> 3)) - (x << 1); }
static long f38(long x) { return ((x * 9 + 38) ^ (x >> 4)) - (x << 1); }
static long f39(long x) { return ((x * 11 + 39) ^ (x >> 5)) - (x << 1); }
static long f40(long x) { return ((x * 3 + 40) ^ (x >> 6)) - (x << 1); }
static long f41(long x) { return ((x * 5 + 41) ^ (x >> 7)) - (x << 1); }
static long f42(long x) { return ((x * 7 + 42) ^ (x >> 1)) - (x << 1); }
static long f43(long x) { return ((x * 9 + 43) ^ (x >> 2)) - (x << 1); }
static long f44(long x) { return ((x * 11 + 44) ^ (x >> 3)) - (x << 1); }
static long f45(long x) { return ((x * 3 + 45) ^ (x >> 4)) - (x << 1); }
static long f46(long x) { return ((x * 5 + 46) ^ (x >> 5)) - (x << 1); }
static long f47(long x) { return ((x * 7 + 47) ^ (x >> 6)) - (x << 1); }
static long f48(long x) { return ((x * 9 + 48) ^ (x >> 7)) - (x << 1); }
static long f49(long x) { return ((x * 11 + 49) ^ (x >> 1)) - (x << 1); }
static long f50(long x) { return ((x * 3 + 50) ^ (x >> 2)) - (x << 1); }
static long f51(long x) { return ((x * 5 + 51) ^ (x >> 3)) - (x << 1); }
static long f52(long x) { return ((x * 7 + 52) ^ (x >> 4)) - (x << 1); }
static long f53(long x) { return ((x * 9 + 53) ^ (x >> 5)) - (x << 1); }
static long f54(long x) { return ((x * 11 + 54) ^ (x >> 6)) - (x << 1); }

void _start(void)
{
    long acc = 1;
    acc += f0(acc + 0);
    acc += f1(acc + 1);
    acc += f2(acc + 2);
    acc += f3(acc + 3);
    acc += f4(acc + 4);
    acc += f5(acc + 5);
    acc += f6(acc + 6);
    acc += f7(acc + 7);
    acc += f8(acc + 8);
    acc += f9(acc + 9);
    acc += f10(acc + 10);
    acc += f11(acc + 11);
    acc += f12(acc + 12);
    acc += f13(acc + 13);
    acc += f14(acc + 14);
    acc += f15(acc + 15);
    acc += f16(acc + 16);
    acc += f17(acc + 17);
    acc += f18(acc + 18);
    acc += f19(acc + 19);
    acc += f20(acc + 20);
    acc += f21(acc + 21);
    acc += f22(acc + 22);
    acc += f23(acc + 23);
    acc += f24(acc + 24);
    acc += f25(acc + 25);
    acc += f26(acc + 26);
    acc += f27(acc + 27);
    acc += f28(acc + 28);
    acc += f29(acc + 29);
    acc += f30(acc + 30);
    acc += f31(acc + 31);
    acc += f32(acc + 32);
    acc += f33(acc + 33);
    acc += f34(acc + 34);
    acc += f35(acc + 35);
    acc += f36(acc + 36);
    acc += f37(acc + 37);
    acc += f38(acc + 38);
    acc += f39(acc + 39);
    acc += f40(acc + 40);
    acc += f41(acc + 41);
    acc += f42(acc + 42);
    acc += f43(acc + 43);
    acc += f44(acc + 44);
    acc += f45(acc + 45);
    acc += f46(acc + 46);
    acc += f47(acc + 47);
    acc += f48(acc + 48);
    acc += f49(acc + 49);
    acc += f50(acc + 50);
    acc += f51(acc + 51);
    acc += f52(acc + 52);
    acc += f53(acc + 53);
    acc += f54(acc + 54);
    static char msg[] = "bench ok X\n";
    msg[9] = 'A' + (acc & 15);
    sys3(1, 1, (long)msg, sizeof msg - 1);
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
