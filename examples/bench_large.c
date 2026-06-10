/* generated benchmark: 1000 functions, freestanding (no libc) */
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
static long f55(long x) { return ((x * 3 + 55) ^ (x >> 7)) - (x << 1); }
static long f56(long x) { return ((x * 5 + 56) ^ (x >> 1)) - (x << 1); }
static long f57(long x) { return ((x * 7 + 57) ^ (x >> 2)) - (x << 1); }
static long f58(long x) { return ((x * 9 + 58) ^ (x >> 3)) - (x << 1); }
static long f59(long x) { return ((x * 11 + 59) ^ (x >> 4)) - (x << 1); }
static long f60(long x) { return ((x * 3 + 60) ^ (x >> 5)) - (x << 1); }
static long f61(long x) { return ((x * 5 + 61) ^ (x >> 6)) - (x << 1); }
static long f62(long x) { return ((x * 7 + 62) ^ (x >> 7)) - (x << 1); }
static long f63(long x) { return ((x * 9 + 63) ^ (x >> 1)) - (x << 1); }
static long f64(long x) { return ((x * 11 + 64) ^ (x >> 2)) - (x << 1); }
static long f65(long x) { return ((x * 3 + 65) ^ (x >> 3)) - (x << 1); }
static long f66(long x) { return ((x * 5 + 66) ^ (x >> 4)) - (x << 1); }
static long f67(long x) { return ((x * 7 + 67) ^ (x >> 5)) - (x << 1); }
static long f68(long x) { return ((x * 9 + 68) ^ (x >> 6)) - (x << 1); }
static long f69(long x) { return ((x * 11 + 69) ^ (x >> 7)) - (x << 1); }
static long f70(long x) { return ((x * 3 + 70) ^ (x >> 1)) - (x << 1); }
static long f71(long x) { return ((x * 5 + 71) ^ (x >> 2)) - (x << 1); }
static long f72(long x) { return ((x * 7 + 72) ^ (x >> 3)) - (x << 1); }
static long f73(long x) { return ((x * 9 + 73) ^ (x >> 4)) - (x << 1); }
static long f74(long x) { return ((x * 11 + 74) ^ (x >> 5)) - (x << 1); }
static long f75(long x) { return ((x * 3 + 75) ^ (x >> 6)) - (x << 1); }
static long f76(long x) { return ((x * 5 + 76) ^ (x >> 7)) - (x << 1); }
static long f77(long x) { return ((x * 7 + 77) ^ (x >> 1)) - (x << 1); }
static long f78(long x) { return ((x * 9 + 78) ^ (x >> 2)) - (x << 1); }
static long f79(long x) { return ((x * 11 + 79) ^ (x >> 3)) - (x << 1); }
static long f80(long x) { return ((x * 3 + 80) ^ (x >> 4)) - (x << 1); }
static long f81(long x) { return ((x * 5 + 81) ^ (x >> 5)) - (x << 1); }
static long f82(long x) { return ((x * 7 + 82) ^ (x >> 6)) - (x << 1); }
static long f83(long x) { return ((x * 9 + 83) ^ (x >> 7)) - (x << 1); }
static long f84(long x) { return ((x * 11 + 84) ^ (x >> 1)) - (x << 1); }
static long f85(long x) { return ((x * 3 + 85) ^ (x >> 2)) - (x << 1); }
static long f86(long x) { return ((x * 5 + 86) ^ (x >> 3)) - (x << 1); }
static long f87(long x) { return ((x * 7 + 87) ^ (x >> 4)) - (x << 1); }
static long f88(long x) { return ((x * 9 + 88) ^ (x >> 5)) - (x << 1); }
static long f89(long x) { return ((x * 11 + 89) ^ (x >> 6)) - (x << 1); }
static long f90(long x) { return ((x * 3 + 90) ^ (x >> 7)) - (x << 1); }
static long f91(long x) { return ((x * 5 + 91) ^ (x >> 1)) - (x << 1); }
static long f92(long x) { return ((x * 7 + 92) ^ (x >> 2)) - (x << 1); }
static long f93(long x) { return ((x * 9 + 93) ^ (x >> 3)) - (x << 1); }
static long f94(long x) { return ((x * 11 + 94) ^ (x >> 4)) - (x << 1); }
static long f95(long x) { return ((x * 3 + 95) ^ (x >> 5)) - (x << 1); }
static long f96(long x) { return ((x * 5 + 96) ^ (x >> 6)) - (x << 1); }
static long f97(long x) { return ((x * 7 + 97) ^ (x >> 7)) - (x << 1); }
static long f98(long x) { return ((x * 9 + 98) ^ (x >> 1)) - (x << 1); }
static long f99(long x) { return ((x * 11 + 99) ^ (x >> 2)) - (x << 1); }
static long f100(long x) { return ((x * 3 + 100) ^ (x >> 3)) - (x << 1); }
static long f101(long x) { return ((x * 5 + 101) ^ (x >> 4)) - (x << 1); }
static long f102(long x) { return ((x * 7 + 102) ^ (x >> 5)) - (x << 1); }
static long f103(long x) { return ((x * 9 + 103) ^ (x >> 6)) - (x << 1); }
static long f104(long x) { return ((x * 11 + 104) ^ (x >> 7)) - (x << 1); }
static long f105(long x) { return ((x * 3 + 105) ^ (x >> 1)) - (x << 1); }
static long f106(long x) { return ((x * 5 + 106) ^ (x >> 2)) - (x << 1); }
static long f107(long x) { return ((x * 7 + 107) ^ (x >> 3)) - (x << 1); }
static long f108(long x) { return ((x * 9 + 108) ^ (x >> 4)) - (x << 1); }
static long f109(long x) { return ((x * 11 + 109) ^ (x >> 5)) - (x << 1); }
static long f110(long x) { return ((x * 3 + 110) ^ (x >> 6)) - (x << 1); }
static long f111(long x) { return ((x * 5 + 111) ^ (x >> 7)) - (x << 1); }
static long f112(long x) { return ((x * 7 + 112) ^ (x >> 1)) - (x << 1); }
static long f113(long x) { return ((x * 9 + 113) ^ (x >> 2)) - (x << 1); }
static long f114(long x) { return ((x * 11 + 114) ^ (x >> 3)) - (x << 1); }
static long f115(long x) { return ((x * 3 + 115) ^ (x >> 4)) - (x << 1); }
static long f116(long x) { return ((x * 5 + 116) ^ (x >> 5)) - (x << 1); }
static long f117(long x) { return ((x * 7 + 117) ^ (x >> 6)) - (x << 1); }
static long f118(long x) { return ((x * 9 + 118) ^ (x >> 7)) - (x << 1); }
static long f119(long x) { return ((x * 11 + 119) ^ (x >> 1)) - (x << 1); }
static long f120(long x) { return ((x * 3 + 120) ^ (x >> 2)) - (x << 1); }
static long f121(long x) { return ((x * 5 + 121) ^ (x >> 3)) - (x << 1); }
static long f122(long x) { return ((x * 7 + 122) ^ (x >> 4)) - (x << 1); }
static long f123(long x) { return ((x * 9 + 123) ^ (x >> 5)) - (x << 1); }
static long f124(long x) { return ((x * 11 + 124) ^ (x >> 6)) - (x << 1); }
static long f125(long x) { return ((x * 3 + 125) ^ (x >> 7)) - (x << 1); }
static long f126(long x) { return ((x * 5 + 126) ^ (x >> 1)) - (x << 1); }
static long f127(long x) { return ((x * 7 + 127) ^ (x >> 2)) - (x << 1); }
static long f128(long x) { return ((x * 9 + 128) ^ (x >> 3)) - (x << 1); }
static long f129(long x) { return ((x * 11 + 129) ^ (x >> 4)) - (x << 1); }
static long f130(long x) { return ((x * 3 + 130) ^ (x >> 5)) - (x << 1); }
static long f131(long x) { return ((x * 5 + 131) ^ (x >> 6)) - (x << 1); }
static long f132(long x) { return ((x * 7 + 132) ^ (x >> 7)) - (x << 1); }
static long f133(long x) { return ((x * 9 + 133) ^ (x >> 1)) - (x << 1); }
static long f134(long x) { return ((x * 11 + 134) ^ (x >> 2)) - (x << 1); }
static long f135(long x) { return ((x * 3 + 135) ^ (x >> 3)) - (x << 1); }
static long f136(long x) { return ((x * 5 + 136) ^ (x >> 4)) - (x << 1); }
static long f137(long x) { return ((x * 7 + 137) ^ (x >> 5)) - (x << 1); }
static long f138(long x) { return ((x * 9 + 138) ^ (x >> 6)) - (x << 1); }
static long f139(long x) { return ((x * 11 + 139) ^ (x >> 7)) - (x << 1); }
static long f140(long x) { return ((x * 3 + 140) ^ (x >> 1)) - (x << 1); }
static long f141(long x) { return ((x * 5 + 141) ^ (x >> 2)) - (x << 1); }
static long f142(long x) { return ((x * 7 + 142) ^ (x >> 3)) - (x << 1); }
static long f143(long x) { return ((x * 9 + 143) ^ (x >> 4)) - (x << 1); }
static long f144(long x) { return ((x * 11 + 144) ^ (x >> 5)) - (x << 1); }
static long f145(long x) { return ((x * 3 + 145) ^ (x >> 6)) - (x << 1); }
static long f146(long x) { return ((x * 5 + 146) ^ (x >> 7)) - (x << 1); }
static long f147(long x) { return ((x * 7 + 147) ^ (x >> 1)) - (x << 1); }
static long f148(long x) { return ((x * 9 + 148) ^ (x >> 2)) - (x << 1); }
static long f149(long x) { return ((x * 11 + 149) ^ (x >> 3)) - (x << 1); }
static long f150(long x) { return ((x * 3 + 150) ^ (x >> 4)) - (x << 1); }
static long f151(long x) { return ((x * 5 + 151) ^ (x >> 5)) - (x << 1); }
static long f152(long x) { return ((x * 7 + 152) ^ (x >> 6)) - (x << 1); }
static long f153(long x) { return ((x * 9 + 153) ^ (x >> 7)) - (x << 1); }
static long f154(long x) { return ((x * 11 + 154) ^ (x >> 1)) - (x << 1); }
static long f155(long x) { return ((x * 3 + 155) ^ (x >> 2)) - (x << 1); }
static long f156(long x) { return ((x * 5 + 156) ^ (x >> 3)) - (x << 1); }
static long f157(long x) { return ((x * 7 + 157) ^ (x >> 4)) - (x << 1); }
static long f158(long x) { return ((x * 9 + 158) ^ (x >> 5)) - (x << 1); }
static long f159(long x) { return ((x * 11 + 159) ^ (x >> 6)) - (x << 1); }
static long f160(long x) { return ((x * 3 + 160) ^ (x >> 7)) - (x << 1); }
static long f161(long x) { return ((x * 5 + 161) ^ (x >> 1)) - (x << 1); }
static long f162(long x) { return ((x * 7 + 162) ^ (x >> 2)) - (x << 1); }
static long f163(long x) { return ((x * 9 + 163) ^ (x >> 3)) - (x << 1); }
static long f164(long x) { return ((x * 11 + 164) ^ (x >> 4)) - (x << 1); }
static long f165(long x) { return ((x * 3 + 165) ^ (x >> 5)) - (x << 1); }
static long f166(long x) { return ((x * 5 + 166) ^ (x >> 6)) - (x << 1); }
static long f167(long x) { return ((x * 7 + 167) ^ (x >> 7)) - (x << 1); }
static long f168(long x) { return ((x * 9 + 168) ^ (x >> 1)) - (x << 1); }
static long f169(long x) { return ((x * 11 + 169) ^ (x >> 2)) - (x << 1); }
static long f170(long x) { return ((x * 3 + 170) ^ (x >> 3)) - (x << 1); }
static long f171(long x) { return ((x * 5 + 171) ^ (x >> 4)) - (x << 1); }
static long f172(long x) { return ((x * 7 + 172) ^ (x >> 5)) - (x << 1); }
static long f173(long x) { return ((x * 9 + 173) ^ (x >> 6)) - (x << 1); }
static long f174(long x) { return ((x * 11 + 174) ^ (x >> 7)) - (x << 1); }
static long f175(long x) { return ((x * 3 + 175) ^ (x >> 1)) - (x << 1); }
static long f176(long x) { return ((x * 5 + 176) ^ (x >> 2)) - (x << 1); }
static long f177(long x) { return ((x * 7 + 177) ^ (x >> 3)) - (x << 1); }
static long f178(long x) { return ((x * 9 + 178) ^ (x >> 4)) - (x << 1); }
static long f179(long x) { return ((x * 11 + 179) ^ (x >> 5)) - (x << 1); }
static long f180(long x) { return ((x * 3 + 180) ^ (x >> 6)) - (x << 1); }
static long f181(long x) { return ((x * 5 + 181) ^ (x >> 7)) - (x << 1); }
static long f182(long x) { return ((x * 7 + 182) ^ (x >> 1)) - (x << 1); }
static long f183(long x) { return ((x * 9 + 183) ^ (x >> 2)) - (x << 1); }
static long f184(long x) { return ((x * 11 + 184) ^ (x >> 3)) - (x << 1); }
static long f185(long x) { return ((x * 3 + 185) ^ (x >> 4)) - (x << 1); }
static long f186(long x) { return ((x * 5 + 186) ^ (x >> 5)) - (x << 1); }
static long f187(long x) { return ((x * 7 + 187) ^ (x >> 6)) - (x << 1); }
static long f188(long x) { return ((x * 9 + 188) ^ (x >> 7)) - (x << 1); }
static long f189(long x) { return ((x * 11 + 189) ^ (x >> 1)) - (x << 1); }
static long f190(long x) { return ((x * 3 + 190) ^ (x >> 2)) - (x << 1); }
static long f191(long x) { return ((x * 5 + 191) ^ (x >> 3)) - (x << 1); }
static long f192(long x) { return ((x * 7 + 192) ^ (x >> 4)) - (x << 1); }
static long f193(long x) { return ((x * 9 + 193) ^ (x >> 5)) - (x << 1); }
static long f194(long x) { return ((x * 11 + 194) ^ (x >> 6)) - (x << 1); }
static long f195(long x) { return ((x * 3 + 195) ^ (x >> 7)) - (x << 1); }
static long f196(long x) { return ((x * 5 + 196) ^ (x >> 1)) - (x << 1); }
static long f197(long x) { return ((x * 7 + 197) ^ (x >> 2)) - (x << 1); }
static long f198(long x) { return ((x * 9 + 198) ^ (x >> 3)) - (x << 1); }
static long f199(long x) { return ((x * 11 + 199) ^ (x >> 4)) - (x << 1); }
static long f200(long x) { return ((x * 3 + 200) ^ (x >> 5)) - (x << 1); }
static long f201(long x) { return ((x * 5 + 201) ^ (x >> 6)) - (x << 1); }
static long f202(long x) { return ((x * 7 + 202) ^ (x >> 7)) - (x << 1); }
static long f203(long x) { return ((x * 9 + 203) ^ (x >> 1)) - (x << 1); }
static long f204(long x) { return ((x * 11 + 204) ^ (x >> 2)) - (x << 1); }
static long f205(long x) { return ((x * 3 + 205) ^ (x >> 3)) - (x << 1); }
static long f206(long x) { return ((x * 5 + 206) ^ (x >> 4)) - (x << 1); }
static long f207(long x) { return ((x * 7 + 207) ^ (x >> 5)) - (x << 1); }
static long f208(long x) { return ((x * 9 + 208) ^ (x >> 6)) - (x << 1); }
static long f209(long x) { return ((x * 11 + 209) ^ (x >> 7)) - (x << 1); }
static long f210(long x) { return ((x * 3 + 210) ^ (x >> 1)) - (x << 1); }
static long f211(long x) { return ((x * 5 + 211) ^ (x >> 2)) - (x << 1); }
static long f212(long x) { return ((x * 7 + 212) ^ (x >> 3)) - (x << 1); }
static long f213(long x) { return ((x * 9 + 213) ^ (x >> 4)) - (x << 1); }
static long f214(long x) { return ((x * 11 + 214) ^ (x >> 5)) - (x << 1); }
static long f215(long x) { return ((x * 3 + 215) ^ (x >> 6)) - (x << 1); }
static long f216(long x) { return ((x * 5 + 216) ^ (x >> 7)) - (x << 1); }
static long f217(long x) { return ((x * 7 + 217) ^ (x >> 1)) - (x << 1); }
static long f218(long x) { return ((x * 9 + 218) ^ (x >> 2)) - (x << 1); }
static long f219(long x) { return ((x * 11 + 219) ^ (x >> 3)) - (x << 1); }
static long f220(long x) { return ((x * 3 + 220) ^ (x >> 4)) - (x << 1); }
static long f221(long x) { return ((x * 5 + 221) ^ (x >> 5)) - (x << 1); }
static long f222(long x) { return ((x * 7 + 222) ^ (x >> 6)) - (x << 1); }
static long f223(long x) { return ((x * 9 + 223) ^ (x >> 7)) - (x << 1); }
static long f224(long x) { return ((x * 11 + 224) ^ (x >> 1)) - (x << 1); }
static long f225(long x) { return ((x * 3 + 225) ^ (x >> 2)) - (x << 1); }
static long f226(long x) { return ((x * 5 + 226) ^ (x >> 3)) - (x << 1); }
static long f227(long x) { return ((x * 7 + 227) ^ (x >> 4)) - (x << 1); }
static long f228(long x) { return ((x * 9 + 228) ^ (x >> 5)) - (x << 1); }
static long f229(long x) { return ((x * 11 + 229) ^ (x >> 6)) - (x << 1); }
static long f230(long x) { return ((x * 3 + 230) ^ (x >> 7)) - (x << 1); }
static long f231(long x) { return ((x * 5 + 231) ^ (x >> 1)) - (x << 1); }
static long f232(long x) { return ((x * 7 + 232) ^ (x >> 2)) - (x << 1); }
static long f233(long x) { return ((x * 9 + 233) ^ (x >> 3)) - (x << 1); }
static long f234(long x) { return ((x * 11 + 234) ^ (x >> 4)) - (x << 1); }
static long f235(long x) { return ((x * 3 + 235) ^ (x >> 5)) - (x << 1); }
static long f236(long x) { return ((x * 5 + 236) ^ (x >> 6)) - (x << 1); }
static long f237(long x) { return ((x * 7 + 237) ^ (x >> 7)) - (x << 1); }
static long f238(long x) { return ((x * 9 + 238) ^ (x >> 1)) - (x << 1); }
static long f239(long x) { return ((x * 11 + 239) ^ (x >> 2)) - (x << 1); }
static long f240(long x) { return ((x * 3 + 240) ^ (x >> 3)) - (x << 1); }
static long f241(long x) { return ((x * 5 + 241) ^ (x >> 4)) - (x << 1); }
static long f242(long x) { return ((x * 7 + 242) ^ (x >> 5)) - (x << 1); }
static long f243(long x) { return ((x * 9 + 243) ^ (x >> 6)) - (x << 1); }
static long f244(long x) { return ((x * 11 + 244) ^ (x >> 7)) - (x << 1); }
static long f245(long x) { return ((x * 3 + 245) ^ (x >> 1)) - (x << 1); }
static long f246(long x) { return ((x * 5 + 246) ^ (x >> 2)) - (x << 1); }
static long f247(long x) { return ((x * 7 + 247) ^ (x >> 3)) - (x << 1); }
static long f248(long x) { return ((x * 9 + 248) ^ (x >> 4)) - (x << 1); }
static long f249(long x) { return ((x * 11 + 249) ^ (x >> 5)) - (x << 1); }
static long f250(long x) { return ((x * 3 + 250) ^ (x >> 6)) - (x << 1); }
static long f251(long x) { return ((x * 5 + 251) ^ (x >> 7)) - (x << 1); }
static long f252(long x) { return ((x * 7 + 252) ^ (x >> 1)) - (x << 1); }
static long f253(long x) { return ((x * 9 + 253) ^ (x >> 2)) - (x << 1); }
static long f254(long x) { return ((x * 11 + 254) ^ (x >> 3)) - (x << 1); }
static long f255(long x) { return ((x * 3 + 255) ^ (x >> 4)) - (x << 1); }
static long f256(long x) { return ((x * 5 + 256) ^ (x >> 5)) - (x << 1); }
static long f257(long x) { return ((x * 7 + 257) ^ (x >> 6)) - (x << 1); }
static long f258(long x) { return ((x * 9 + 258) ^ (x >> 7)) - (x << 1); }
static long f259(long x) { return ((x * 11 + 259) ^ (x >> 1)) - (x << 1); }
static long f260(long x) { return ((x * 3 + 260) ^ (x >> 2)) - (x << 1); }
static long f261(long x) { return ((x * 5 + 261) ^ (x >> 3)) - (x << 1); }
static long f262(long x) { return ((x * 7 + 262) ^ (x >> 4)) - (x << 1); }
static long f263(long x) { return ((x * 9 + 263) ^ (x >> 5)) - (x << 1); }
static long f264(long x) { return ((x * 11 + 264) ^ (x >> 6)) - (x << 1); }
static long f265(long x) { return ((x * 3 + 265) ^ (x >> 7)) - (x << 1); }
static long f266(long x) { return ((x * 5 + 266) ^ (x >> 1)) - (x << 1); }
static long f267(long x) { return ((x * 7 + 267) ^ (x >> 2)) - (x << 1); }
static long f268(long x) { return ((x * 9 + 268) ^ (x >> 3)) - (x << 1); }
static long f269(long x) { return ((x * 11 + 269) ^ (x >> 4)) - (x << 1); }
static long f270(long x) { return ((x * 3 + 270) ^ (x >> 5)) - (x << 1); }
static long f271(long x) { return ((x * 5 + 271) ^ (x >> 6)) - (x << 1); }
static long f272(long x) { return ((x * 7 + 272) ^ (x >> 7)) - (x << 1); }
static long f273(long x) { return ((x * 9 + 273) ^ (x >> 1)) - (x << 1); }
static long f274(long x) { return ((x * 11 + 274) ^ (x >> 2)) - (x << 1); }
static long f275(long x) { return ((x * 3 + 275) ^ (x >> 3)) - (x << 1); }
static long f276(long x) { return ((x * 5 + 276) ^ (x >> 4)) - (x << 1); }
static long f277(long x) { return ((x * 7 + 277) ^ (x >> 5)) - (x << 1); }
static long f278(long x) { return ((x * 9 + 278) ^ (x >> 6)) - (x << 1); }
static long f279(long x) { return ((x * 11 + 279) ^ (x >> 7)) - (x << 1); }
static long f280(long x) { return ((x * 3 + 280) ^ (x >> 1)) - (x << 1); }
static long f281(long x) { return ((x * 5 + 281) ^ (x >> 2)) - (x << 1); }
static long f282(long x) { return ((x * 7 + 282) ^ (x >> 3)) - (x << 1); }
static long f283(long x) { return ((x * 9 + 283) ^ (x >> 4)) - (x << 1); }
static long f284(long x) { return ((x * 11 + 284) ^ (x >> 5)) - (x << 1); }
static long f285(long x) { return ((x * 3 + 285) ^ (x >> 6)) - (x << 1); }
static long f286(long x) { return ((x * 5 + 286) ^ (x >> 7)) - (x << 1); }
static long f287(long x) { return ((x * 7 + 287) ^ (x >> 1)) - (x << 1); }
static long f288(long x) { return ((x * 9 + 288) ^ (x >> 2)) - (x << 1); }
static long f289(long x) { return ((x * 11 + 289) ^ (x >> 3)) - (x << 1); }
static long f290(long x) { return ((x * 3 + 290) ^ (x >> 4)) - (x << 1); }
static long f291(long x) { return ((x * 5 + 291) ^ (x >> 5)) - (x << 1); }
static long f292(long x) { return ((x * 7 + 292) ^ (x >> 6)) - (x << 1); }
static long f293(long x) { return ((x * 9 + 293) ^ (x >> 7)) - (x << 1); }
static long f294(long x) { return ((x * 11 + 294) ^ (x >> 1)) - (x << 1); }
static long f295(long x) { return ((x * 3 + 295) ^ (x >> 2)) - (x << 1); }
static long f296(long x) { return ((x * 5 + 296) ^ (x >> 3)) - (x << 1); }
static long f297(long x) { return ((x * 7 + 297) ^ (x >> 4)) - (x << 1); }
static long f298(long x) { return ((x * 9 + 298) ^ (x >> 5)) - (x << 1); }
static long f299(long x) { return ((x * 11 + 299) ^ (x >> 6)) - (x << 1); }
static long f300(long x) { return ((x * 3 + 300) ^ (x >> 7)) - (x << 1); }
static long f301(long x) { return ((x * 5 + 301) ^ (x >> 1)) - (x << 1); }
static long f302(long x) { return ((x * 7 + 302) ^ (x >> 2)) - (x << 1); }
static long f303(long x) { return ((x * 9 + 303) ^ (x >> 3)) - (x << 1); }
static long f304(long x) { return ((x * 11 + 304) ^ (x >> 4)) - (x << 1); }
static long f305(long x) { return ((x * 3 + 305) ^ (x >> 5)) - (x << 1); }
static long f306(long x) { return ((x * 5 + 306) ^ (x >> 6)) - (x << 1); }
static long f307(long x) { return ((x * 7 + 307) ^ (x >> 7)) - (x << 1); }
static long f308(long x) { return ((x * 9 + 308) ^ (x >> 1)) - (x << 1); }
static long f309(long x) { return ((x * 11 + 309) ^ (x >> 2)) - (x << 1); }
static long f310(long x) { return ((x * 3 + 310) ^ (x >> 3)) - (x << 1); }
static long f311(long x) { return ((x * 5 + 311) ^ (x >> 4)) - (x << 1); }
static long f312(long x) { return ((x * 7 + 312) ^ (x >> 5)) - (x << 1); }
static long f313(long x) { return ((x * 9 + 313) ^ (x >> 6)) - (x << 1); }
static long f314(long x) { return ((x * 11 + 314) ^ (x >> 7)) - (x << 1); }
static long f315(long x) { return ((x * 3 + 315) ^ (x >> 1)) - (x << 1); }
static long f316(long x) { return ((x * 5 + 316) ^ (x >> 2)) - (x << 1); }
static long f317(long x) { return ((x * 7 + 317) ^ (x >> 3)) - (x << 1); }
static long f318(long x) { return ((x * 9 + 318) ^ (x >> 4)) - (x << 1); }
static long f319(long x) { return ((x * 11 + 319) ^ (x >> 5)) - (x << 1); }
static long f320(long x) { return ((x * 3 + 320) ^ (x >> 6)) - (x << 1); }
static long f321(long x) { return ((x * 5 + 321) ^ (x >> 7)) - (x << 1); }
static long f322(long x) { return ((x * 7 + 322) ^ (x >> 1)) - (x << 1); }
static long f323(long x) { return ((x * 9 + 323) ^ (x >> 2)) - (x << 1); }
static long f324(long x) { return ((x * 11 + 324) ^ (x >> 3)) - (x << 1); }
static long f325(long x) { return ((x * 3 + 325) ^ (x >> 4)) - (x << 1); }
static long f326(long x) { return ((x * 5 + 326) ^ (x >> 5)) - (x << 1); }
static long f327(long x) { return ((x * 7 + 327) ^ (x >> 6)) - (x << 1); }
static long f328(long x) { return ((x * 9 + 328) ^ (x >> 7)) - (x << 1); }
static long f329(long x) { return ((x * 11 + 329) ^ (x >> 1)) - (x << 1); }
static long f330(long x) { return ((x * 3 + 330) ^ (x >> 2)) - (x << 1); }
static long f331(long x) { return ((x * 5 + 331) ^ (x >> 3)) - (x << 1); }
static long f332(long x) { return ((x * 7 + 332) ^ (x >> 4)) - (x << 1); }
static long f333(long x) { return ((x * 9 + 333) ^ (x >> 5)) - (x << 1); }
static long f334(long x) { return ((x * 11 + 334) ^ (x >> 6)) - (x << 1); }
static long f335(long x) { return ((x * 3 + 335) ^ (x >> 7)) - (x << 1); }
static long f336(long x) { return ((x * 5 + 336) ^ (x >> 1)) - (x << 1); }
static long f337(long x) { return ((x * 7 + 337) ^ (x >> 2)) - (x << 1); }
static long f338(long x) { return ((x * 9 + 338) ^ (x >> 3)) - (x << 1); }
static long f339(long x) { return ((x * 11 + 339) ^ (x >> 4)) - (x << 1); }
static long f340(long x) { return ((x * 3 + 340) ^ (x >> 5)) - (x << 1); }
static long f341(long x) { return ((x * 5 + 341) ^ (x >> 6)) - (x << 1); }
static long f342(long x) { return ((x * 7 + 342) ^ (x >> 7)) - (x << 1); }
static long f343(long x) { return ((x * 9 + 343) ^ (x >> 1)) - (x << 1); }
static long f344(long x) { return ((x * 11 + 344) ^ (x >> 2)) - (x << 1); }
static long f345(long x) { return ((x * 3 + 345) ^ (x >> 3)) - (x << 1); }
static long f346(long x) { return ((x * 5 + 346) ^ (x >> 4)) - (x << 1); }
static long f347(long x) { return ((x * 7 + 347) ^ (x >> 5)) - (x << 1); }
static long f348(long x) { return ((x * 9 + 348) ^ (x >> 6)) - (x << 1); }
static long f349(long x) { return ((x * 11 + 349) ^ (x >> 7)) - (x << 1); }
static long f350(long x) { return ((x * 3 + 350) ^ (x >> 1)) - (x << 1); }
static long f351(long x) { return ((x * 5 + 351) ^ (x >> 2)) - (x << 1); }
static long f352(long x) { return ((x * 7 + 352) ^ (x >> 3)) - (x << 1); }
static long f353(long x) { return ((x * 9 + 353) ^ (x >> 4)) - (x << 1); }
static long f354(long x) { return ((x * 11 + 354) ^ (x >> 5)) - (x << 1); }
static long f355(long x) { return ((x * 3 + 355) ^ (x >> 6)) - (x << 1); }
static long f356(long x) { return ((x * 5 + 356) ^ (x >> 7)) - (x << 1); }
static long f357(long x) { return ((x * 7 + 357) ^ (x >> 1)) - (x << 1); }
static long f358(long x) { return ((x * 9 + 358) ^ (x >> 2)) - (x << 1); }
static long f359(long x) { return ((x * 11 + 359) ^ (x >> 3)) - (x << 1); }
static long f360(long x) { return ((x * 3 + 360) ^ (x >> 4)) - (x << 1); }
static long f361(long x) { return ((x * 5 + 361) ^ (x >> 5)) - (x << 1); }
static long f362(long x) { return ((x * 7 + 362) ^ (x >> 6)) - (x << 1); }
static long f363(long x) { return ((x * 9 + 363) ^ (x >> 7)) - (x << 1); }
static long f364(long x) { return ((x * 11 + 364) ^ (x >> 1)) - (x << 1); }
static long f365(long x) { return ((x * 3 + 365) ^ (x >> 2)) - (x << 1); }
static long f366(long x) { return ((x * 5 + 366) ^ (x >> 3)) - (x << 1); }
static long f367(long x) { return ((x * 7 + 367) ^ (x >> 4)) - (x << 1); }
static long f368(long x) { return ((x * 9 + 368) ^ (x >> 5)) - (x << 1); }
static long f369(long x) { return ((x * 11 + 369) ^ (x >> 6)) - (x << 1); }
static long f370(long x) { return ((x * 3 + 370) ^ (x >> 7)) - (x << 1); }
static long f371(long x) { return ((x * 5 + 371) ^ (x >> 1)) - (x << 1); }
static long f372(long x) { return ((x * 7 + 372) ^ (x >> 2)) - (x << 1); }
static long f373(long x) { return ((x * 9 + 373) ^ (x >> 3)) - (x << 1); }
static long f374(long x) { return ((x * 11 + 374) ^ (x >> 4)) - (x << 1); }
static long f375(long x) { return ((x * 3 + 375) ^ (x >> 5)) - (x << 1); }
static long f376(long x) { return ((x * 5 + 376) ^ (x >> 6)) - (x << 1); }
static long f377(long x) { return ((x * 7 + 377) ^ (x >> 7)) - (x << 1); }
static long f378(long x) { return ((x * 9 + 378) ^ (x >> 1)) - (x << 1); }
static long f379(long x) { return ((x * 11 + 379) ^ (x >> 2)) - (x << 1); }
static long f380(long x) { return ((x * 3 + 380) ^ (x >> 3)) - (x << 1); }
static long f381(long x) { return ((x * 5 + 381) ^ (x >> 4)) - (x << 1); }
static long f382(long x) { return ((x * 7 + 382) ^ (x >> 5)) - (x << 1); }
static long f383(long x) { return ((x * 9 + 383) ^ (x >> 6)) - (x << 1); }
static long f384(long x) { return ((x * 11 + 384) ^ (x >> 7)) - (x << 1); }
static long f385(long x) { return ((x * 3 + 385) ^ (x >> 1)) - (x << 1); }
static long f386(long x) { return ((x * 5 + 386) ^ (x >> 2)) - (x << 1); }
static long f387(long x) { return ((x * 7 + 387) ^ (x >> 3)) - (x << 1); }
static long f388(long x) { return ((x * 9 + 388) ^ (x >> 4)) - (x << 1); }
static long f389(long x) { return ((x * 11 + 389) ^ (x >> 5)) - (x << 1); }
static long f390(long x) { return ((x * 3 + 390) ^ (x >> 6)) - (x << 1); }
static long f391(long x) { return ((x * 5 + 391) ^ (x >> 7)) - (x << 1); }
static long f392(long x) { return ((x * 7 + 392) ^ (x >> 1)) - (x << 1); }
static long f393(long x) { return ((x * 9 + 393) ^ (x >> 2)) - (x << 1); }
static long f394(long x) { return ((x * 11 + 394) ^ (x >> 3)) - (x << 1); }
static long f395(long x) { return ((x * 3 + 395) ^ (x >> 4)) - (x << 1); }
static long f396(long x) { return ((x * 5 + 396) ^ (x >> 5)) - (x << 1); }
static long f397(long x) { return ((x * 7 + 397) ^ (x >> 6)) - (x << 1); }
static long f398(long x) { return ((x * 9 + 398) ^ (x >> 7)) - (x << 1); }
static long f399(long x) { return ((x * 11 + 399) ^ (x >> 1)) - (x << 1); }
static long f400(long x) { return ((x * 3 + 400) ^ (x >> 2)) - (x << 1); }
static long f401(long x) { return ((x * 5 + 401) ^ (x >> 3)) - (x << 1); }
static long f402(long x) { return ((x * 7 + 402) ^ (x >> 4)) - (x << 1); }
static long f403(long x) { return ((x * 9 + 403) ^ (x >> 5)) - (x << 1); }
static long f404(long x) { return ((x * 11 + 404) ^ (x >> 6)) - (x << 1); }
static long f405(long x) { return ((x * 3 + 405) ^ (x >> 7)) - (x << 1); }
static long f406(long x) { return ((x * 5 + 406) ^ (x >> 1)) - (x << 1); }
static long f407(long x) { return ((x * 7 + 407) ^ (x >> 2)) - (x << 1); }
static long f408(long x) { return ((x * 9 + 408) ^ (x >> 3)) - (x << 1); }
static long f409(long x) { return ((x * 11 + 409) ^ (x >> 4)) - (x << 1); }
static long f410(long x) { return ((x * 3 + 410) ^ (x >> 5)) - (x << 1); }
static long f411(long x) { return ((x * 5 + 411) ^ (x >> 6)) - (x << 1); }
static long f412(long x) { return ((x * 7 + 412) ^ (x >> 7)) - (x << 1); }
static long f413(long x) { return ((x * 9 + 413) ^ (x >> 1)) - (x << 1); }
static long f414(long x) { return ((x * 11 + 414) ^ (x >> 2)) - (x << 1); }
static long f415(long x) { return ((x * 3 + 415) ^ (x >> 3)) - (x << 1); }
static long f416(long x) { return ((x * 5 + 416) ^ (x >> 4)) - (x << 1); }
static long f417(long x) { return ((x * 7 + 417) ^ (x >> 5)) - (x << 1); }
static long f418(long x) { return ((x * 9 + 418) ^ (x >> 6)) - (x << 1); }
static long f419(long x) { return ((x * 11 + 419) ^ (x >> 7)) - (x << 1); }
static long f420(long x) { return ((x * 3 + 420) ^ (x >> 1)) - (x << 1); }
static long f421(long x) { return ((x * 5 + 421) ^ (x >> 2)) - (x << 1); }
static long f422(long x) { return ((x * 7 + 422) ^ (x >> 3)) - (x << 1); }
static long f423(long x) { return ((x * 9 + 423) ^ (x >> 4)) - (x << 1); }
static long f424(long x) { return ((x * 11 + 424) ^ (x >> 5)) - (x << 1); }
static long f425(long x) { return ((x * 3 + 425) ^ (x >> 6)) - (x << 1); }
static long f426(long x) { return ((x * 5 + 426) ^ (x >> 7)) - (x << 1); }
static long f427(long x) { return ((x * 7 + 427) ^ (x >> 1)) - (x << 1); }
static long f428(long x) { return ((x * 9 + 428) ^ (x >> 2)) - (x << 1); }
static long f429(long x) { return ((x * 11 + 429) ^ (x >> 3)) - (x << 1); }
static long f430(long x) { return ((x * 3 + 430) ^ (x >> 4)) - (x << 1); }
static long f431(long x) { return ((x * 5 + 431) ^ (x >> 5)) - (x << 1); }
static long f432(long x) { return ((x * 7 + 432) ^ (x >> 6)) - (x << 1); }
static long f433(long x) { return ((x * 9 + 433) ^ (x >> 7)) - (x << 1); }
static long f434(long x) { return ((x * 11 + 434) ^ (x >> 1)) - (x << 1); }
static long f435(long x) { return ((x * 3 + 435) ^ (x >> 2)) - (x << 1); }
static long f436(long x) { return ((x * 5 + 436) ^ (x >> 3)) - (x << 1); }
static long f437(long x) { return ((x * 7 + 437) ^ (x >> 4)) - (x << 1); }
static long f438(long x) { return ((x * 9 + 438) ^ (x >> 5)) - (x << 1); }
static long f439(long x) { return ((x * 11 + 439) ^ (x >> 6)) - (x << 1); }
static long f440(long x) { return ((x * 3 + 440) ^ (x >> 7)) - (x << 1); }
static long f441(long x) { return ((x * 5 + 441) ^ (x >> 1)) - (x << 1); }
static long f442(long x) { return ((x * 7 + 442) ^ (x >> 2)) - (x << 1); }
static long f443(long x) { return ((x * 9 + 443) ^ (x >> 3)) - (x << 1); }
static long f444(long x) { return ((x * 11 + 444) ^ (x >> 4)) - (x << 1); }
static long f445(long x) { return ((x * 3 + 445) ^ (x >> 5)) - (x << 1); }
static long f446(long x) { return ((x * 5 + 446) ^ (x >> 6)) - (x << 1); }
static long f447(long x) { return ((x * 7 + 447) ^ (x >> 7)) - (x << 1); }
static long f448(long x) { return ((x * 9 + 448) ^ (x >> 1)) - (x << 1); }
static long f449(long x) { return ((x * 11 + 449) ^ (x >> 2)) - (x << 1); }
static long f450(long x) { return ((x * 3 + 450) ^ (x >> 3)) - (x << 1); }
static long f451(long x) { return ((x * 5 + 451) ^ (x >> 4)) - (x << 1); }
static long f452(long x) { return ((x * 7 + 452) ^ (x >> 5)) - (x << 1); }
static long f453(long x) { return ((x * 9 + 453) ^ (x >> 6)) - (x << 1); }
static long f454(long x) { return ((x * 11 + 454) ^ (x >> 7)) - (x << 1); }
static long f455(long x) { return ((x * 3 + 455) ^ (x >> 1)) - (x << 1); }
static long f456(long x) { return ((x * 5 + 456) ^ (x >> 2)) - (x << 1); }
static long f457(long x) { return ((x * 7 + 457) ^ (x >> 3)) - (x << 1); }
static long f458(long x) { return ((x * 9 + 458) ^ (x >> 4)) - (x << 1); }
static long f459(long x) { return ((x * 11 + 459) ^ (x >> 5)) - (x << 1); }
static long f460(long x) { return ((x * 3 + 460) ^ (x >> 6)) - (x << 1); }
static long f461(long x) { return ((x * 5 + 461) ^ (x >> 7)) - (x << 1); }
static long f462(long x) { return ((x * 7 + 462) ^ (x >> 1)) - (x << 1); }
static long f463(long x) { return ((x * 9 + 463) ^ (x >> 2)) - (x << 1); }
static long f464(long x) { return ((x * 11 + 464) ^ (x >> 3)) - (x << 1); }
static long f465(long x) { return ((x * 3 + 465) ^ (x >> 4)) - (x << 1); }
static long f466(long x) { return ((x * 5 + 466) ^ (x >> 5)) - (x << 1); }
static long f467(long x) { return ((x * 7 + 467) ^ (x >> 6)) - (x << 1); }
static long f468(long x) { return ((x * 9 + 468) ^ (x >> 7)) - (x << 1); }
static long f469(long x) { return ((x * 11 + 469) ^ (x >> 1)) - (x << 1); }
static long f470(long x) { return ((x * 3 + 470) ^ (x >> 2)) - (x << 1); }
static long f471(long x) { return ((x * 5 + 471) ^ (x >> 3)) - (x << 1); }
static long f472(long x) { return ((x * 7 + 472) ^ (x >> 4)) - (x << 1); }
static long f473(long x) { return ((x * 9 + 473) ^ (x >> 5)) - (x << 1); }
static long f474(long x) { return ((x * 11 + 474) ^ (x >> 6)) - (x << 1); }
static long f475(long x) { return ((x * 3 + 475) ^ (x >> 7)) - (x << 1); }
static long f476(long x) { return ((x * 5 + 476) ^ (x >> 1)) - (x << 1); }
static long f477(long x) { return ((x * 7 + 477) ^ (x >> 2)) - (x << 1); }
static long f478(long x) { return ((x * 9 + 478) ^ (x >> 3)) - (x << 1); }
static long f479(long x) { return ((x * 11 + 479) ^ (x >> 4)) - (x << 1); }
static long f480(long x) { return ((x * 3 + 480) ^ (x >> 5)) - (x << 1); }
static long f481(long x) { return ((x * 5 + 481) ^ (x >> 6)) - (x << 1); }
static long f482(long x) { return ((x * 7 + 482) ^ (x >> 7)) - (x << 1); }
static long f483(long x) { return ((x * 9 + 483) ^ (x >> 1)) - (x << 1); }
static long f484(long x) { return ((x * 11 + 484) ^ (x >> 2)) - (x << 1); }
static long f485(long x) { return ((x * 3 + 485) ^ (x >> 3)) - (x << 1); }
static long f486(long x) { return ((x * 5 + 486) ^ (x >> 4)) - (x << 1); }
static long f487(long x) { return ((x * 7 + 487) ^ (x >> 5)) - (x << 1); }
static long f488(long x) { return ((x * 9 + 488) ^ (x >> 6)) - (x << 1); }
static long f489(long x) { return ((x * 11 + 489) ^ (x >> 7)) - (x << 1); }
static long f490(long x) { return ((x * 3 + 490) ^ (x >> 1)) - (x << 1); }
static long f491(long x) { return ((x * 5 + 491) ^ (x >> 2)) - (x << 1); }
static long f492(long x) { return ((x * 7 + 492) ^ (x >> 3)) - (x << 1); }
static long f493(long x) { return ((x * 9 + 493) ^ (x >> 4)) - (x << 1); }
static long f494(long x) { return ((x * 11 + 494) ^ (x >> 5)) - (x << 1); }
static long f495(long x) { return ((x * 3 + 495) ^ (x >> 6)) - (x << 1); }
static long f496(long x) { return ((x * 5 + 496) ^ (x >> 7)) - (x << 1); }
static long f497(long x) { return ((x * 7 + 497) ^ (x >> 1)) - (x << 1); }
static long f498(long x) { return ((x * 9 + 498) ^ (x >> 2)) - (x << 1); }
static long f499(long x) { return ((x * 11 + 499) ^ (x >> 3)) - (x << 1); }
static long f500(long x) { return ((x * 3 + 500) ^ (x >> 4)) - (x << 1); }
static long f501(long x) { return ((x * 5 + 501) ^ (x >> 5)) - (x << 1); }
static long f502(long x) { return ((x * 7 + 502) ^ (x >> 6)) - (x << 1); }
static long f503(long x) { return ((x * 9 + 503) ^ (x >> 7)) - (x << 1); }
static long f504(long x) { return ((x * 11 + 504) ^ (x >> 1)) - (x << 1); }
static long f505(long x) { return ((x * 3 + 505) ^ (x >> 2)) - (x << 1); }
static long f506(long x) { return ((x * 5 + 506) ^ (x >> 3)) - (x << 1); }
static long f507(long x) { return ((x * 7 + 507) ^ (x >> 4)) - (x << 1); }
static long f508(long x) { return ((x * 9 + 508) ^ (x >> 5)) - (x << 1); }
static long f509(long x) { return ((x * 11 + 509) ^ (x >> 6)) - (x << 1); }
static long f510(long x) { return ((x * 3 + 510) ^ (x >> 7)) - (x << 1); }
static long f511(long x) { return ((x * 5 + 511) ^ (x >> 1)) - (x << 1); }
static long f512(long x) { return ((x * 7 + 512) ^ (x >> 2)) - (x << 1); }
static long f513(long x) { return ((x * 9 + 513) ^ (x >> 3)) - (x << 1); }
static long f514(long x) { return ((x * 11 + 514) ^ (x >> 4)) - (x << 1); }
static long f515(long x) { return ((x * 3 + 515) ^ (x >> 5)) - (x << 1); }
static long f516(long x) { return ((x * 5 + 516) ^ (x >> 6)) - (x << 1); }
static long f517(long x) { return ((x * 7 + 517) ^ (x >> 7)) - (x << 1); }
static long f518(long x) { return ((x * 9 + 518) ^ (x >> 1)) - (x << 1); }
static long f519(long x) { return ((x * 11 + 519) ^ (x >> 2)) - (x << 1); }
static long f520(long x) { return ((x * 3 + 520) ^ (x >> 3)) - (x << 1); }
static long f521(long x) { return ((x * 5 + 521) ^ (x >> 4)) - (x << 1); }
static long f522(long x) { return ((x * 7 + 522) ^ (x >> 5)) - (x << 1); }
static long f523(long x) { return ((x * 9 + 523) ^ (x >> 6)) - (x << 1); }
static long f524(long x) { return ((x * 11 + 524) ^ (x >> 7)) - (x << 1); }
static long f525(long x) { return ((x * 3 + 525) ^ (x >> 1)) - (x << 1); }
static long f526(long x) { return ((x * 5 + 526) ^ (x >> 2)) - (x << 1); }
static long f527(long x) { return ((x * 7 + 527) ^ (x >> 3)) - (x << 1); }
static long f528(long x) { return ((x * 9 + 528) ^ (x >> 4)) - (x << 1); }
static long f529(long x) { return ((x * 11 + 529) ^ (x >> 5)) - (x << 1); }
static long f530(long x) { return ((x * 3 + 530) ^ (x >> 6)) - (x << 1); }
static long f531(long x) { return ((x * 5 + 531) ^ (x >> 7)) - (x << 1); }
static long f532(long x) { return ((x * 7 + 532) ^ (x >> 1)) - (x << 1); }
static long f533(long x) { return ((x * 9 + 533) ^ (x >> 2)) - (x << 1); }
static long f534(long x) { return ((x * 11 + 534) ^ (x >> 3)) - (x << 1); }
static long f535(long x) { return ((x * 3 + 535) ^ (x >> 4)) - (x << 1); }
static long f536(long x) { return ((x * 5 + 536) ^ (x >> 5)) - (x << 1); }
static long f537(long x) { return ((x * 7 + 537) ^ (x >> 6)) - (x << 1); }
static long f538(long x) { return ((x * 9 + 538) ^ (x >> 7)) - (x << 1); }
static long f539(long x) { return ((x * 11 + 539) ^ (x >> 1)) - (x << 1); }
static long f540(long x) { return ((x * 3 + 540) ^ (x >> 2)) - (x << 1); }
static long f541(long x) { return ((x * 5 + 541) ^ (x >> 3)) - (x << 1); }
static long f542(long x) { return ((x * 7 + 542) ^ (x >> 4)) - (x << 1); }
static long f543(long x) { return ((x * 9 + 543) ^ (x >> 5)) - (x << 1); }
static long f544(long x) { return ((x * 11 + 544) ^ (x >> 6)) - (x << 1); }
static long f545(long x) { return ((x * 3 + 545) ^ (x >> 7)) - (x << 1); }
static long f546(long x) { return ((x * 5 + 546) ^ (x >> 1)) - (x << 1); }
static long f547(long x) { return ((x * 7 + 547) ^ (x >> 2)) - (x << 1); }
static long f548(long x) { return ((x * 9 + 548) ^ (x >> 3)) - (x << 1); }
static long f549(long x) { return ((x * 11 + 549) ^ (x >> 4)) - (x << 1); }
static long f550(long x) { return ((x * 3 + 550) ^ (x >> 5)) - (x << 1); }
static long f551(long x) { return ((x * 5 + 551) ^ (x >> 6)) - (x << 1); }
static long f552(long x) { return ((x * 7 + 552) ^ (x >> 7)) - (x << 1); }
static long f553(long x) { return ((x * 9 + 553) ^ (x >> 1)) - (x << 1); }
static long f554(long x) { return ((x * 11 + 554) ^ (x >> 2)) - (x << 1); }
static long f555(long x) { return ((x * 3 + 555) ^ (x >> 3)) - (x << 1); }
static long f556(long x) { return ((x * 5 + 556) ^ (x >> 4)) - (x << 1); }
static long f557(long x) { return ((x * 7 + 557) ^ (x >> 5)) - (x << 1); }
static long f558(long x) { return ((x * 9 + 558) ^ (x >> 6)) - (x << 1); }
static long f559(long x) { return ((x * 11 + 559) ^ (x >> 7)) - (x << 1); }
static long f560(long x) { return ((x * 3 + 560) ^ (x >> 1)) - (x << 1); }
static long f561(long x) { return ((x * 5 + 561) ^ (x >> 2)) - (x << 1); }
static long f562(long x) { return ((x * 7 + 562) ^ (x >> 3)) - (x << 1); }
static long f563(long x) { return ((x * 9 + 563) ^ (x >> 4)) - (x << 1); }
static long f564(long x) { return ((x * 11 + 564) ^ (x >> 5)) - (x << 1); }
static long f565(long x) { return ((x * 3 + 565) ^ (x >> 6)) - (x << 1); }
static long f566(long x) { return ((x * 5 + 566) ^ (x >> 7)) - (x << 1); }
static long f567(long x) { return ((x * 7 + 567) ^ (x >> 1)) - (x << 1); }
static long f568(long x) { return ((x * 9 + 568) ^ (x >> 2)) - (x << 1); }
static long f569(long x) { return ((x * 11 + 569) ^ (x >> 3)) - (x << 1); }
static long f570(long x) { return ((x * 3 + 570) ^ (x >> 4)) - (x << 1); }
static long f571(long x) { return ((x * 5 + 571) ^ (x >> 5)) - (x << 1); }
static long f572(long x) { return ((x * 7 + 572) ^ (x >> 6)) - (x << 1); }
static long f573(long x) { return ((x * 9 + 573) ^ (x >> 7)) - (x << 1); }
static long f574(long x) { return ((x * 11 + 574) ^ (x >> 1)) - (x << 1); }
static long f575(long x) { return ((x * 3 + 575) ^ (x >> 2)) - (x << 1); }
static long f576(long x) { return ((x * 5 + 576) ^ (x >> 3)) - (x << 1); }
static long f577(long x) { return ((x * 7 + 577) ^ (x >> 4)) - (x << 1); }
static long f578(long x) { return ((x * 9 + 578) ^ (x >> 5)) - (x << 1); }
static long f579(long x) { return ((x * 11 + 579) ^ (x >> 6)) - (x << 1); }
static long f580(long x) { return ((x * 3 + 580) ^ (x >> 7)) - (x << 1); }
static long f581(long x) { return ((x * 5 + 581) ^ (x >> 1)) - (x << 1); }
static long f582(long x) { return ((x * 7 + 582) ^ (x >> 2)) - (x << 1); }
static long f583(long x) { return ((x * 9 + 583) ^ (x >> 3)) - (x << 1); }
static long f584(long x) { return ((x * 11 + 584) ^ (x >> 4)) - (x << 1); }
static long f585(long x) { return ((x * 3 + 585) ^ (x >> 5)) - (x << 1); }
static long f586(long x) { return ((x * 5 + 586) ^ (x >> 6)) - (x << 1); }
static long f587(long x) { return ((x * 7 + 587) ^ (x >> 7)) - (x << 1); }
static long f588(long x) { return ((x * 9 + 588) ^ (x >> 1)) - (x << 1); }
static long f589(long x) { return ((x * 11 + 589) ^ (x >> 2)) - (x << 1); }
static long f590(long x) { return ((x * 3 + 590) ^ (x >> 3)) - (x << 1); }
static long f591(long x) { return ((x * 5 + 591) ^ (x >> 4)) - (x << 1); }
static long f592(long x) { return ((x * 7 + 592) ^ (x >> 5)) - (x << 1); }
static long f593(long x) { return ((x * 9 + 593) ^ (x >> 6)) - (x << 1); }
static long f594(long x) { return ((x * 11 + 594) ^ (x >> 7)) - (x << 1); }
static long f595(long x) { return ((x * 3 + 595) ^ (x >> 1)) - (x << 1); }
static long f596(long x) { return ((x * 5 + 596) ^ (x >> 2)) - (x << 1); }
static long f597(long x) { return ((x * 7 + 597) ^ (x >> 3)) - (x << 1); }
static long f598(long x) { return ((x * 9 + 598) ^ (x >> 4)) - (x << 1); }
static long f599(long x) { return ((x * 11 + 599) ^ (x >> 5)) - (x << 1); }
static long f600(long x) { return ((x * 3 + 600) ^ (x >> 6)) - (x << 1); }
static long f601(long x) { return ((x * 5 + 601) ^ (x >> 7)) - (x << 1); }
static long f602(long x) { return ((x * 7 + 602) ^ (x >> 1)) - (x << 1); }
static long f603(long x) { return ((x * 9 + 603) ^ (x >> 2)) - (x << 1); }
static long f604(long x) { return ((x * 11 + 604) ^ (x >> 3)) - (x << 1); }
static long f605(long x) { return ((x * 3 + 605) ^ (x >> 4)) - (x << 1); }
static long f606(long x) { return ((x * 5 + 606) ^ (x >> 5)) - (x << 1); }
static long f607(long x) { return ((x * 7 + 607) ^ (x >> 6)) - (x << 1); }
static long f608(long x) { return ((x * 9 + 608) ^ (x >> 7)) - (x << 1); }
static long f609(long x) { return ((x * 11 + 609) ^ (x >> 1)) - (x << 1); }
static long f610(long x) { return ((x * 3 + 610) ^ (x >> 2)) - (x << 1); }
static long f611(long x) { return ((x * 5 + 611) ^ (x >> 3)) - (x << 1); }
static long f612(long x) { return ((x * 7 + 612) ^ (x >> 4)) - (x << 1); }
static long f613(long x) { return ((x * 9 + 613) ^ (x >> 5)) - (x << 1); }
static long f614(long x) { return ((x * 11 + 614) ^ (x >> 6)) - (x << 1); }
static long f615(long x) { return ((x * 3 + 615) ^ (x >> 7)) - (x << 1); }
static long f616(long x) { return ((x * 5 + 616) ^ (x >> 1)) - (x << 1); }
static long f617(long x) { return ((x * 7 + 617) ^ (x >> 2)) - (x << 1); }
static long f618(long x) { return ((x * 9 + 618) ^ (x >> 3)) - (x << 1); }
static long f619(long x) { return ((x * 11 + 619) ^ (x >> 4)) - (x << 1); }
static long f620(long x) { return ((x * 3 + 620) ^ (x >> 5)) - (x << 1); }
static long f621(long x) { return ((x * 5 + 621) ^ (x >> 6)) - (x << 1); }
static long f622(long x) { return ((x * 7 + 622) ^ (x >> 7)) - (x << 1); }
static long f623(long x) { return ((x * 9 + 623) ^ (x >> 1)) - (x << 1); }
static long f624(long x) { return ((x * 11 + 624) ^ (x >> 2)) - (x << 1); }
static long f625(long x) { return ((x * 3 + 625) ^ (x >> 3)) - (x << 1); }
static long f626(long x) { return ((x * 5 + 626) ^ (x >> 4)) - (x << 1); }
static long f627(long x) { return ((x * 7 + 627) ^ (x >> 5)) - (x << 1); }
static long f628(long x) { return ((x * 9 + 628) ^ (x >> 6)) - (x << 1); }
static long f629(long x) { return ((x * 11 + 629) ^ (x >> 7)) - (x << 1); }
static long f630(long x) { return ((x * 3 + 630) ^ (x >> 1)) - (x << 1); }
static long f631(long x) { return ((x * 5 + 631) ^ (x >> 2)) - (x << 1); }
static long f632(long x) { return ((x * 7 + 632) ^ (x >> 3)) - (x << 1); }
static long f633(long x) { return ((x * 9 + 633) ^ (x >> 4)) - (x << 1); }
static long f634(long x) { return ((x * 11 + 634) ^ (x >> 5)) - (x << 1); }
static long f635(long x) { return ((x * 3 + 635) ^ (x >> 6)) - (x << 1); }
static long f636(long x) { return ((x * 5 + 636) ^ (x >> 7)) - (x << 1); }
static long f637(long x) { return ((x * 7 + 637) ^ (x >> 1)) - (x << 1); }
static long f638(long x) { return ((x * 9 + 638) ^ (x >> 2)) - (x << 1); }
static long f639(long x) { return ((x * 11 + 639) ^ (x >> 3)) - (x << 1); }
static long f640(long x) { return ((x * 3 + 640) ^ (x >> 4)) - (x << 1); }
static long f641(long x) { return ((x * 5 + 641) ^ (x >> 5)) - (x << 1); }
static long f642(long x) { return ((x * 7 + 642) ^ (x >> 6)) - (x << 1); }
static long f643(long x) { return ((x * 9 + 643) ^ (x >> 7)) - (x << 1); }
static long f644(long x) { return ((x * 11 + 644) ^ (x >> 1)) - (x << 1); }
static long f645(long x) { return ((x * 3 + 645) ^ (x >> 2)) - (x << 1); }
static long f646(long x) { return ((x * 5 + 646) ^ (x >> 3)) - (x << 1); }
static long f647(long x) { return ((x * 7 + 647) ^ (x >> 4)) - (x << 1); }
static long f648(long x) { return ((x * 9 + 648) ^ (x >> 5)) - (x << 1); }
static long f649(long x) { return ((x * 11 + 649) ^ (x >> 6)) - (x << 1); }
static long f650(long x) { return ((x * 3 + 650) ^ (x >> 7)) - (x << 1); }
static long f651(long x) { return ((x * 5 + 651) ^ (x >> 1)) - (x << 1); }
static long f652(long x) { return ((x * 7 + 652) ^ (x >> 2)) - (x << 1); }
static long f653(long x) { return ((x * 9 + 653) ^ (x >> 3)) - (x << 1); }
static long f654(long x) { return ((x * 11 + 654) ^ (x >> 4)) - (x << 1); }
static long f655(long x) { return ((x * 3 + 655) ^ (x >> 5)) - (x << 1); }
static long f656(long x) { return ((x * 5 + 656) ^ (x >> 6)) - (x << 1); }
static long f657(long x) { return ((x * 7 + 657) ^ (x >> 7)) - (x << 1); }
static long f658(long x) { return ((x * 9 + 658) ^ (x >> 1)) - (x << 1); }
static long f659(long x) { return ((x * 11 + 659) ^ (x >> 2)) - (x << 1); }
static long f660(long x) { return ((x * 3 + 660) ^ (x >> 3)) - (x << 1); }
static long f661(long x) { return ((x * 5 + 661) ^ (x >> 4)) - (x << 1); }
static long f662(long x) { return ((x * 7 + 662) ^ (x >> 5)) - (x << 1); }
static long f663(long x) { return ((x * 9 + 663) ^ (x >> 6)) - (x << 1); }
static long f664(long x) { return ((x * 11 + 664) ^ (x >> 7)) - (x << 1); }
static long f665(long x) { return ((x * 3 + 665) ^ (x >> 1)) - (x << 1); }
static long f666(long x) { return ((x * 5 + 666) ^ (x >> 2)) - (x << 1); }
static long f667(long x) { return ((x * 7 + 667) ^ (x >> 3)) - (x << 1); }
static long f668(long x) { return ((x * 9 + 668) ^ (x >> 4)) - (x << 1); }
static long f669(long x) { return ((x * 11 + 669) ^ (x >> 5)) - (x << 1); }
static long f670(long x) { return ((x * 3 + 670) ^ (x >> 6)) - (x << 1); }
static long f671(long x) { return ((x * 5 + 671) ^ (x >> 7)) - (x << 1); }
static long f672(long x) { return ((x * 7 + 672) ^ (x >> 1)) - (x << 1); }
static long f673(long x) { return ((x * 9 + 673) ^ (x >> 2)) - (x << 1); }
static long f674(long x) { return ((x * 11 + 674) ^ (x >> 3)) - (x << 1); }
static long f675(long x) { return ((x * 3 + 675) ^ (x >> 4)) - (x << 1); }
static long f676(long x) { return ((x * 5 + 676) ^ (x >> 5)) - (x << 1); }
static long f677(long x) { return ((x * 7 + 677) ^ (x >> 6)) - (x << 1); }
static long f678(long x) { return ((x * 9 + 678) ^ (x >> 7)) - (x << 1); }
static long f679(long x) { return ((x * 11 + 679) ^ (x >> 1)) - (x << 1); }
static long f680(long x) { return ((x * 3 + 680) ^ (x >> 2)) - (x << 1); }
static long f681(long x) { return ((x * 5 + 681) ^ (x >> 3)) - (x << 1); }
static long f682(long x) { return ((x * 7 + 682) ^ (x >> 4)) - (x << 1); }
static long f683(long x) { return ((x * 9 + 683) ^ (x >> 5)) - (x << 1); }
static long f684(long x) { return ((x * 11 + 684) ^ (x >> 6)) - (x << 1); }
static long f685(long x) { return ((x * 3 + 685) ^ (x >> 7)) - (x << 1); }
static long f686(long x) { return ((x * 5 + 686) ^ (x >> 1)) - (x << 1); }
static long f687(long x) { return ((x * 7 + 687) ^ (x >> 2)) - (x << 1); }
static long f688(long x) { return ((x * 9 + 688) ^ (x >> 3)) - (x << 1); }
static long f689(long x) { return ((x * 11 + 689) ^ (x >> 4)) - (x << 1); }
static long f690(long x) { return ((x * 3 + 690) ^ (x >> 5)) - (x << 1); }
static long f691(long x) { return ((x * 5 + 691) ^ (x >> 6)) - (x << 1); }
static long f692(long x) { return ((x * 7 + 692) ^ (x >> 7)) - (x << 1); }
static long f693(long x) { return ((x * 9 + 693) ^ (x >> 1)) - (x << 1); }
static long f694(long x) { return ((x * 11 + 694) ^ (x >> 2)) - (x << 1); }
static long f695(long x) { return ((x * 3 + 695) ^ (x >> 3)) - (x << 1); }
static long f696(long x) { return ((x * 5 + 696) ^ (x >> 4)) - (x << 1); }
static long f697(long x) { return ((x * 7 + 697) ^ (x >> 5)) - (x << 1); }
static long f698(long x) { return ((x * 9 + 698) ^ (x >> 6)) - (x << 1); }
static long f699(long x) { return ((x * 11 + 699) ^ (x >> 7)) - (x << 1); }
static long f700(long x) { return ((x * 3 + 700) ^ (x >> 1)) - (x << 1); }
static long f701(long x) { return ((x * 5 + 701) ^ (x >> 2)) - (x << 1); }
static long f702(long x) { return ((x * 7 + 702) ^ (x >> 3)) - (x << 1); }
static long f703(long x) { return ((x * 9 + 703) ^ (x >> 4)) - (x << 1); }
static long f704(long x) { return ((x * 11 + 704) ^ (x >> 5)) - (x << 1); }
static long f705(long x) { return ((x * 3 + 705) ^ (x >> 6)) - (x << 1); }
static long f706(long x) { return ((x * 5 + 706) ^ (x >> 7)) - (x << 1); }
static long f707(long x) { return ((x * 7 + 707) ^ (x >> 1)) - (x << 1); }
static long f708(long x) { return ((x * 9 + 708) ^ (x >> 2)) - (x << 1); }
static long f709(long x) { return ((x * 11 + 709) ^ (x >> 3)) - (x << 1); }
static long f710(long x) { return ((x * 3 + 710) ^ (x >> 4)) - (x << 1); }
static long f711(long x) { return ((x * 5 + 711) ^ (x >> 5)) - (x << 1); }
static long f712(long x) { return ((x * 7 + 712) ^ (x >> 6)) - (x << 1); }
static long f713(long x) { return ((x * 9 + 713) ^ (x >> 7)) - (x << 1); }
static long f714(long x) { return ((x * 11 + 714) ^ (x >> 1)) - (x << 1); }
static long f715(long x) { return ((x * 3 + 715) ^ (x >> 2)) - (x << 1); }
static long f716(long x) { return ((x * 5 + 716) ^ (x >> 3)) - (x << 1); }
static long f717(long x) { return ((x * 7 + 717) ^ (x >> 4)) - (x << 1); }
static long f718(long x) { return ((x * 9 + 718) ^ (x >> 5)) - (x << 1); }
static long f719(long x) { return ((x * 11 + 719) ^ (x >> 6)) - (x << 1); }
static long f720(long x) { return ((x * 3 + 720) ^ (x >> 7)) - (x << 1); }
static long f721(long x) { return ((x * 5 + 721) ^ (x >> 1)) - (x << 1); }
static long f722(long x) { return ((x * 7 + 722) ^ (x >> 2)) - (x << 1); }
static long f723(long x) { return ((x * 9 + 723) ^ (x >> 3)) - (x << 1); }
static long f724(long x) { return ((x * 11 + 724) ^ (x >> 4)) - (x << 1); }
static long f725(long x) { return ((x * 3 + 725) ^ (x >> 5)) - (x << 1); }
static long f726(long x) { return ((x * 5 + 726) ^ (x >> 6)) - (x << 1); }
static long f727(long x) { return ((x * 7 + 727) ^ (x >> 7)) - (x << 1); }
static long f728(long x) { return ((x * 9 + 728) ^ (x >> 1)) - (x << 1); }
static long f729(long x) { return ((x * 11 + 729) ^ (x >> 2)) - (x << 1); }
static long f730(long x) { return ((x * 3 + 730) ^ (x >> 3)) - (x << 1); }
static long f731(long x) { return ((x * 5 + 731) ^ (x >> 4)) - (x << 1); }
static long f732(long x) { return ((x * 7 + 732) ^ (x >> 5)) - (x << 1); }
static long f733(long x) { return ((x * 9 + 733) ^ (x >> 6)) - (x << 1); }
static long f734(long x) { return ((x * 11 + 734) ^ (x >> 7)) - (x << 1); }
static long f735(long x) { return ((x * 3 + 735) ^ (x >> 1)) - (x << 1); }
static long f736(long x) { return ((x * 5 + 736) ^ (x >> 2)) - (x << 1); }
static long f737(long x) { return ((x * 7 + 737) ^ (x >> 3)) - (x << 1); }
static long f738(long x) { return ((x * 9 + 738) ^ (x >> 4)) - (x << 1); }
static long f739(long x) { return ((x * 11 + 739) ^ (x >> 5)) - (x << 1); }
static long f740(long x) { return ((x * 3 + 740) ^ (x >> 6)) - (x << 1); }
static long f741(long x) { return ((x * 5 + 741) ^ (x >> 7)) - (x << 1); }
static long f742(long x) { return ((x * 7 + 742) ^ (x >> 1)) - (x << 1); }
static long f743(long x) { return ((x * 9 + 743) ^ (x >> 2)) - (x << 1); }
static long f744(long x) { return ((x * 11 + 744) ^ (x >> 3)) - (x << 1); }
static long f745(long x) { return ((x * 3 + 745) ^ (x >> 4)) - (x << 1); }
static long f746(long x) { return ((x * 5 + 746) ^ (x >> 5)) - (x << 1); }
static long f747(long x) { return ((x * 7 + 747) ^ (x >> 6)) - (x << 1); }
static long f748(long x) { return ((x * 9 + 748) ^ (x >> 7)) - (x << 1); }
static long f749(long x) { return ((x * 11 + 749) ^ (x >> 1)) - (x << 1); }
static long f750(long x) { return ((x * 3 + 750) ^ (x >> 2)) - (x << 1); }
static long f751(long x) { return ((x * 5 + 751) ^ (x >> 3)) - (x << 1); }
static long f752(long x) { return ((x * 7 + 752) ^ (x >> 4)) - (x << 1); }
static long f753(long x) { return ((x * 9 + 753) ^ (x >> 5)) - (x << 1); }
static long f754(long x) { return ((x * 11 + 754) ^ (x >> 6)) - (x << 1); }
static long f755(long x) { return ((x * 3 + 755) ^ (x >> 7)) - (x << 1); }
static long f756(long x) { return ((x * 5 + 756) ^ (x >> 1)) - (x << 1); }
static long f757(long x) { return ((x * 7 + 757) ^ (x >> 2)) - (x << 1); }
static long f758(long x) { return ((x * 9 + 758) ^ (x >> 3)) - (x << 1); }
static long f759(long x) { return ((x * 11 + 759) ^ (x >> 4)) - (x << 1); }
static long f760(long x) { return ((x * 3 + 760) ^ (x >> 5)) - (x << 1); }
static long f761(long x) { return ((x * 5 + 761) ^ (x >> 6)) - (x << 1); }
static long f762(long x) { return ((x * 7 + 762) ^ (x >> 7)) - (x << 1); }
static long f763(long x) { return ((x * 9 + 763) ^ (x >> 1)) - (x << 1); }
static long f764(long x) { return ((x * 11 + 764) ^ (x >> 2)) - (x << 1); }
static long f765(long x) { return ((x * 3 + 765) ^ (x >> 3)) - (x << 1); }
static long f766(long x) { return ((x * 5 + 766) ^ (x >> 4)) - (x << 1); }
static long f767(long x) { return ((x * 7 + 767) ^ (x >> 5)) - (x << 1); }
static long f768(long x) { return ((x * 9 + 768) ^ (x >> 6)) - (x << 1); }
static long f769(long x) { return ((x * 11 + 769) ^ (x >> 7)) - (x << 1); }
static long f770(long x) { return ((x * 3 + 770) ^ (x >> 1)) - (x << 1); }
static long f771(long x) { return ((x * 5 + 771) ^ (x >> 2)) - (x << 1); }
static long f772(long x) { return ((x * 7 + 772) ^ (x >> 3)) - (x << 1); }
static long f773(long x) { return ((x * 9 + 773) ^ (x >> 4)) - (x << 1); }
static long f774(long x) { return ((x * 11 + 774) ^ (x >> 5)) - (x << 1); }
static long f775(long x) { return ((x * 3 + 775) ^ (x >> 6)) - (x << 1); }
static long f776(long x) { return ((x * 5 + 776) ^ (x >> 7)) - (x << 1); }
static long f777(long x) { return ((x * 7 + 777) ^ (x >> 1)) - (x << 1); }
static long f778(long x) { return ((x * 9 + 778) ^ (x >> 2)) - (x << 1); }
static long f779(long x) { return ((x * 11 + 779) ^ (x >> 3)) - (x << 1); }
static long f780(long x) { return ((x * 3 + 780) ^ (x >> 4)) - (x << 1); }
static long f781(long x) { return ((x * 5 + 781) ^ (x >> 5)) - (x << 1); }
static long f782(long x) { return ((x * 7 + 782) ^ (x >> 6)) - (x << 1); }
static long f783(long x) { return ((x * 9 + 783) ^ (x >> 7)) - (x << 1); }
static long f784(long x) { return ((x * 11 + 784) ^ (x >> 1)) - (x << 1); }
static long f785(long x) { return ((x * 3 + 785) ^ (x >> 2)) - (x << 1); }
static long f786(long x) { return ((x * 5 + 786) ^ (x >> 3)) - (x << 1); }
static long f787(long x) { return ((x * 7 + 787) ^ (x >> 4)) - (x << 1); }
static long f788(long x) { return ((x * 9 + 788) ^ (x >> 5)) - (x << 1); }
static long f789(long x) { return ((x * 11 + 789) ^ (x >> 6)) - (x << 1); }
static long f790(long x) { return ((x * 3 + 790) ^ (x >> 7)) - (x << 1); }
static long f791(long x) { return ((x * 5 + 791) ^ (x >> 1)) - (x << 1); }
static long f792(long x) { return ((x * 7 + 792) ^ (x >> 2)) - (x << 1); }
static long f793(long x) { return ((x * 9 + 793) ^ (x >> 3)) - (x << 1); }
static long f794(long x) { return ((x * 11 + 794) ^ (x >> 4)) - (x << 1); }
static long f795(long x) { return ((x * 3 + 795) ^ (x >> 5)) - (x << 1); }
static long f796(long x) { return ((x * 5 + 796) ^ (x >> 6)) - (x << 1); }
static long f797(long x) { return ((x * 7 + 797) ^ (x >> 7)) - (x << 1); }
static long f798(long x) { return ((x * 9 + 798) ^ (x >> 1)) - (x << 1); }
static long f799(long x) { return ((x * 11 + 799) ^ (x >> 2)) - (x << 1); }
static long f800(long x) { return ((x * 3 + 800) ^ (x >> 3)) - (x << 1); }
static long f801(long x) { return ((x * 5 + 801) ^ (x >> 4)) - (x << 1); }
static long f802(long x) { return ((x * 7 + 802) ^ (x >> 5)) - (x << 1); }
static long f803(long x) { return ((x * 9 + 803) ^ (x >> 6)) - (x << 1); }
static long f804(long x) { return ((x * 11 + 804) ^ (x >> 7)) - (x << 1); }
static long f805(long x) { return ((x * 3 + 805) ^ (x >> 1)) - (x << 1); }
static long f806(long x) { return ((x * 5 + 806) ^ (x >> 2)) - (x << 1); }
static long f807(long x) { return ((x * 7 + 807) ^ (x >> 3)) - (x << 1); }
static long f808(long x) { return ((x * 9 + 808) ^ (x >> 4)) - (x << 1); }
static long f809(long x) { return ((x * 11 + 809) ^ (x >> 5)) - (x << 1); }
static long f810(long x) { return ((x * 3 + 810) ^ (x >> 6)) - (x << 1); }
static long f811(long x) { return ((x * 5 + 811) ^ (x >> 7)) - (x << 1); }
static long f812(long x) { return ((x * 7 + 812) ^ (x >> 1)) - (x << 1); }
static long f813(long x) { return ((x * 9 + 813) ^ (x >> 2)) - (x << 1); }
static long f814(long x) { return ((x * 11 + 814) ^ (x >> 3)) - (x << 1); }
static long f815(long x) { return ((x * 3 + 815) ^ (x >> 4)) - (x << 1); }
static long f816(long x) { return ((x * 5 + 816) ^ (x >> 5)) - (x << 1); }
static long f817(long x) { return ((x * 7 + 817) ^ (x >> 6)) - (x << 1); }
static long f818(long x) { return ((x * 9 + 818) ^ (x >> 7)) - (x << 1); }
static long f819(long x) { return ((x * 11 + 819) ^ (x >> 1)) - (x << 1); }
static long f820(long x) { return ((x * 3 + 820) ^ (x >> 2)) - (x << 1); }
static long f821(long x) { return ((x * 5 + 821) ^ (x >> 3)) - (x << 1); }
static long f822(long x) { return ((x * 7 + 822) ^ (x >> 4)) - (x << 1); }
static long f823(long x) { return ((x * 9 + 823) ^ (x >> 5)) - (x << 1); }
static long f824(long x) { return ((x * 11 + 824) ^ (x >> 6)) - (x << 1); }
static long f825(long x) { return ((x * 3 + 825) ^ (x >> 7)) - (x << 1); }
static long f826(long x) { return ((x * 5 + 826) ^ (x >> 1)) - (x << 1); }
static long f827(long x) { return ((x * 7 + 827) ^ (x >> 2)) - (x << 1); }
static long f828(long x) { return ((x * 9 + 828) ^ (x >> 3)) - (x << 1); }
static long f829(long x) { return ((x * 11 + 829) ^ (x >> 4)) - (x << 1); }
static long f830(long x) { return ((x * 3 + 830) ^ (x >> 5)) - (x << 1); }
static long f831(long x) { return ((x * 5 + 831) ^ (x >> 6)) - (x << 1); }
static long f832(long x) { return ((x * 7 + 832) ^ (x >> 7)) - (x << 1); }
static long f833(long x) { return ((x * 9 + 833) ^ (x >> 1)) - (x << 1); }
static long f834(long x) { return ((x * 11 + 834) ^ (x >> 2)) - (x << 1); }
static long f835(long x) { return ((x * 3 + 835) ^ (x >> 3)) - (x << 1); }
static long f836(long x) { return ((x * 5 + 836) ^ (x >> 4)) - (x << 1); }
static long f837(long x) { return ((x * 7 + 837) ^ (x >> 5)) - (x << 1); }
static long f838(long x) { return ((x * 9 + 838) ^ (x >> 6)) - (x << 1); }
static long f839(long x) { return ((x * 11 + 839) ^ (x >> 7)) - (x << 1); }
static long f840(long x) { return ((x * 3 + 840) ^ (x >> 1)) - (x << 1); }
static long f841(long x) { return ((x * 5 + 841) ^ (x >> 2)) - (x << 1); }
static long f842(long x) { return ((x * 7 + 842) ^ (x >> 3)) - (x << 1); }
static long f843(long x) { return ((x * 9 + 843) ^ (x >> 4)) - (x << 1); }
static long f844(long x) { return ((x * 11 + 844) ^ (x >> 5)) - (x << 1); }
static long f845(long x) { return ((x * 3 + 845) ^ (x >> 6)) - (x << 1); }
static long f846(long x) { return ((x * 5 + 846) ^ (x >> 7)) - (x << 1); }
static long f847(long x) { return ((x * 7 + 847) ^ (x >> 1)) - (x << 1); }
static long f848(long x) { return ((x * 9 + 848) ^ (x >> 2)) - (x << 1); }
static long f849(long x) { return ((x * 11 + 849) ^ (x >> 3)) - (x << 1); }
static long f850(long x) { return ((x * 3 + 850) ^ (x >> 4)) - (x << 1); }
static long f851(long x) { return ((x * 5 + 851) ^ (x >> 5)) - (x << 1); }
static long f852(long x) { return ((x * 7 + 852) ^ (x >> 6)) - (x << 1); }
static long f853(long x) { return ((x * 9 + 853) ^ (x >> 7)) - (x << 1); }
static long f854(long x) { return ((x * 11 + 854) ^ (x >> 1)) - (x << 1); }
static long f855(long x) { return ((x * 3 + 855) ^ (x >> 2)) - (x << 1); }
static long f856(long x) { return ((x * 5 + 856) ^ (x >> 3)) - (x << 1); }
static long f857(long x) { return ((x * 7 + 857) ^ (x >> 4)) - (x << 1); }
static long f858(long x) { return ((x * 9 + 858) ^ (x >> 5)) - (x << 1); }
static long f859(long x) { return ((x * 11 + 859) ^ (x >> 6)) - (x << 1); }
static long f860(long x) { return ((x * 3 + 860) ^ (x >> 7)) - (x << 1); }
static long f861(long x) { return ((x * 5 + 861) ^ (x >> 1)) - (x << 1); }
static long f862(long x) { return ((x * 7 + 862) ^ (x >> 2)) - (x << 1); }
static long f863(long x) { return ((x * 9 + 863) ^ (x >> 3)) - (x << 1); }
static long f864(long x) { return ((x * 11 + 864) ^ (x >> 4)) - (x << 1); }
static long f865(long x) { return ((x * 3 + 865) ^ (x >> 5)) - (x << 1); }
static long f866(long x) { return ((x * 5 + 866) ^ (x >> 6)) - (x << 1); }
static long f867(long x) { return ((x * 7 + 867) ^ (x >> 7)) - (x << 1); }
static long f868(long x) { return ((x * 9 + 868) ^ (x >> 1)) - (x << 1); }
static long f869(long x) { return ((x * 11 + 869) ^ (x >> 2)) - (x << 1); }
static long f870(long x) { return ((x * 3 + 870) ^ (x >> 3)) - (x << 1); }
static long f871(long x) { return ((x * 5 + 871) ^ (x >> 4)) - (x << 1); }
static long f872(long x) { return ((x * 7 + 872) ^ (x >> 5)) - (x << 1); }
static long f873(long x) { return ((x * 9 + 873) ^ (x >> 6)) - (x << 1); }
static long f874(long x) { return ((x * 11 + 874) ^ (x >> 7)) - (x << 1); }
static long f875(long x) { return ((x * 3 + 875) ^ (x >> 1)) - (x << 1); }
static long f876(long x) { return ((x * 5 + 876) ^ (x >> 2)) - (x << 1); }
static long f877(long x) { return ((x * 7 + 877) ^ (x >> 3)) - (x << 1); }
static long f878(long x) { return ((x * 9 + 878) ^ (x >> 4)) - (x << 1); }
static long f879(long x) { return ((x * 11 + 879) ^ (x >> 5)) - (x << 1); }
static long f880(long x) { return ((x * 3 + 880) ^ (x >> 6)) - (x << 1); }
static long f881(long x) { return ((x * 5 + 881) ^ (x >> 7)) - (x << 1); }
static long f882(long x) { return ((x * 7 + 882) ^ (x >> 1)) - (x << 1); }
static long f883(long x) { return ((x * 9 + 883) ^ (x >> 2)) - (x << 1); }
static long f884(long x) { return ((x * 11 + 884) ^ (x >> 3)) - (x << 1); }
static long f885(long x) { return ((x * 3 + 885) ^ (x >> 4)) - (x << 1); }
static long f886(long x) { return ((x * 5 + 886) ^ (x >> 5)) - (x << 1); }
static long f887(long x) { return ((x * 7 + 887) ^ (x >> 6)) - (x << 1); }
static long f888(long x) { return ((x * 9 + 888) ^ (x >> 7)) - (x << 1); }
static long f889(long x) { return ((x * 11 + 889) ^ (x >> 1)) - (x << 1); }
static long f890(long x) { return ((x * 3 + 890) ^ (x >> 2)) - (x << 1); }
static long f891(long x) { return ((x * 5 + 891) ^ (x >> 3)) - (x << 1); }
static long f892(long x) { return ((x * 7 + 892) ^ (x >> 4)) - (x << 1); }
static long f893(long x) { return ((x * 9 + 893) ^ (x >> 5)) - (x << 1); }
static long f894(long x) { return ((x * 11 + 894) ^ (x >> 6)) - (x << 1); }
static long f895(long x) { return ((x * 3 + 895) ^ (x >> 7)) - (x << 1); }
static long f896(long x) { return ((x * 5 + 896) ^ (x >> 1)) - (x << 1); }
static long f897(long x) { return ((x * 7 + 897) ^ (x >> 2)) - (x << 1); }
static long f898(long x) { return ((x * 9 + 898) ^ (x >> 3)) - (x << 1); }
static long f899(long x) { return ((x * 11 + 899) ^ (x >> 4)) - (x << 1); }
static long f900(long x) { return ((x * 3 + 900) ^ (x >> 5)) - (x << 1); }
static long f901(long x) { return ((x * 5 + 901) ^ (x >> 6)) - (x << 1); }
static long f902(long x) { return ((x * 7 + 902) ^ (x >> 7)) - (x << 1); }
static long f903(long x) { return ((x * 9 + 903) ^ (x >> 1)) - (x << 1); }
static long f904(long x) { return ((x * 11 + 904) ^ (x >> 2)) - (x << 1); }
static long f905(long x) { return ((x * 3 + 905) ^ (x >> 3)) - (x << 1); }
static long f906(long x) { return ((x * 5 + 906) ^ (x >> 4)) - (x << 1); }
static long f907(long x) { return ((x * 7 + 907) ^ (x >> 5)) - (x << 1); }
static long f908(long x) { return ((x * 9 + 908) ^ (x >> 6)) - (x << 1); }
static long f909(long x) { return ((x * 11 + 909) ^ (x >> 7)) - (x << 1); }
static long f910(long x) { return ((x * 3 + 910) ^ (x >> 1)) - (x << 1); }
static long f911(long x) { return ((x * 5 + 911) ^ (x >> 2)) - (x << 1); }
static long f912(long x) { return ((x * 7 + 912) ^ (x >> 3)) - (x << 1); }
static long f913(long x) { return ((x * 9 + 913) ^ (x >> 4)) - (x << 1); }
static long f914(long x) { return ((x * 11 + 914) ^ (x >> 5)) - (x << 1); }
static long f915(long x) { return ((x * 3 + 915) ^ (x >> 6)) - (x << 1); }
static long f916(long x) { return ((x * 5 + 916) ^ (x >> 7)) - (x << 1); }
static long f917(long x) { return ((x * 7 + 917) ^ (x >> 1)) - (x << 1); }
static long f918(long x) { return ((x * 9 + 918) ^ (x >> 2)) - (x << 1); }
static long f919(long x) { return ((x * 11 + 919) ^ (x >> 3)) - (x << 1); }
static long f920(long x) { return ((x * 3 + 920) ^ (x >> 4)) - (x << 1); }
static long f921(long x) { return ((x * 5 + 921) ^ (x >> 5)) - (x << 1); }
static long f922(long x) { return ((x * 7 + 922) ^ (x >> 6)) - (x << 1); }
static long f923(long x) { return ((x * 9 + 923) ^ (x >> 7)) - (x << 1); }
static long f924(long x) { return ((x * 11 + 924) ^ (x >> 1)) - (x << 1); }
static long f925(long x) { return ((x * 3 + 925) ^ (x >> 2)) - (x << 1); }
static long f926(long x) { return ((x * 5 + 926) ^ (x >> 3)) - (x << 1); }
static long f927(long x) { return ((x * 7 + 927) ^ (x >> 4)) - (x << 1); }
static long f928(long x) { return ((x * 9 + 928) ^ (x >> 5)) - (x << 1); }
static long f929(long x) { return ((x * 11 + 929) ^ (x >> 6)) - (x << 1); }
static long f930(long x) { return ((x * 3 + 930) ^ (x >> 7)) - (x << 1); }
static long f931(long x) { return ((x * 5 + 931) ^ (x >> 1)) - (x << 1); }
static long f932(long x) { return ((x * 7 + 932) ^ (x >> 2)) - (x << 1); }
static long f933(long x) { return ((x * 9 + 933) ^ (x >> 3)) - (x << 1); }
static long f934(long x) { return ((x * 11 + 934) ^ (x >> 4)) - (x << 1); }
static long f935(long x) { return ((x * 3 + 935) ^ (x >> 5)) - (x << 1); }
static long f936(long x) { return ((x * 5 + 936) ^ (x >> 6)) - (x << 1); }
static long f937(long x) { return ((x * 7 + 937) ^ (x >> 7)) - (x << 1); }
static long f938(long x) { return ((x * 9 + 938) ^ (x >> 1)) - (x << 1); }
static long f939(long x) { return ((x * 11 + 939) ^ (x >> 2)) - (x << 1); }
static long f940(long x) { return ((x * 3 + 940) ^ (x >> 3)) - (x << 1); }
static long f941(long x) { return ((x * 5 + 941) ^ (x >> 4)) - (x << 1); }
static long f942(long x) { return ((x * 7 + 942) ^ (x >> 5)) - (x << 1); }
static long f943(long x) { return ((x * 9 + 943) ^ (x >> 6)) - (x << 1); }
static long f944(long x) { return ((x * 11 + 944) ^ (x >> 7)) - (x << 1); }
static long f945(long x) { return ((x * 3 + 945) ^ (x >> 1)) - (x << 1); }
static long f946(long x) { return ((x * 5 + 946) ^ (x >> 2)) - (x << 1); }
static long f947(long x) { return ((x * 7 + 947) ^ (x >> 3)) - (x << 1); }
static long f948(long x) { return ((x * 9 + 948) ^ (x >> 4)) - (x << 1); }
static long f949(long x) { return ((x * 11 + 949) ^ (x >> 5)) - (x << 1); }
static long f950(long x) { return ((x * 3 + 950) ^ (x >> 6)) - (x << 1); }
static long f951(long x) { return ((x * 5 + 951) ^ (x >> 7)) - (x << 1); }
static long f952(long x) { return ((x * 7 + 952) ^ (x >> 1)) - (x << 1); }
static long f953(long x) { return ((x * 9 + 953) ^ (x >> 2)) - (x << 1); }
static long f954(long x) { return ((x * 11 + 954) ^ (x >> 3)) - (x << 1); }
static long f955(long x) { return ((x * 3 + 955) ^ (x >> 4)) - (x << 1); }
static long f956(long x) { return ((x * 5 + 956) ^ (x >> 5)) - (x << 1); }
static long f957(long x) { return ((x * 7 + 957) ^ (x >> 6)) - (x << 1); }
static long f958(long x) { return ((x * 9 + 958) ^ (x >> 7)) - (x << 1); }
static long f959(long x) { return ((x * 11 + 959) ^ (x >> 1)) - (x << 1); }
static long f960(long x) { return ((x * 3 + 960) ^ (x >> 2)) - (x << 1); }
static long f961(long x) { return ((x * 5 + 961) ^ (x >> 3)) - (x << 1); }
static long f962(long x) { return ((x * 7 + 962) ^ (x >> 4)) - (x << 1); }
static long f963(long x) { return ((x * 9 + 963) ^ (x >> 5)) - (x << 1); }
static long f964(long x) { return ((x * 11 + 964) ^ (x >> 6)) - (x << 1); }
static long f965(long x) { return ((x * 3 + 965) ^ (x >> 7)) - (x << 1); }
static long f966(long x) { return ((x * 5 + 966) ^ (x >> 1)) - (x << 1); }
static long f967(long x) { return ((x * 7 + 967) ^ (x >> 2)) - (x << 1); }
static long f968(long x) { return ((x * 9 + 968) ^ (x >> 3)) - (x << 1); }
static long f969(long x) { return ((x * 11 + 969) ^ (x >> 4)) - (x << 1); }
static long f970(long x) { return ((x * 3 + 970) ^ (x >> 5)) - (x << 1); }
static long f971(long x) { return ((x * 5 + 971) ^ (x >> 6)) - (x << 1); }
static long f972(long x) { return ((x * 7 + 972) ^ (x >> 7)) - (x << 1); }
static long f973(long x) { return ((x * 9 + 973) ^ (x >> 1)) - (x << 1); }
static long f974(long x) { return ((x * 11 + 974) ^ (x >> 2)) - (x << 1); }
static long f975(long x) { return ((x * 3 + 975) ^ (x >> 3)) - (x << 1); }
static long f976(long x) { return ((x * 5 + 976) ^ (x >> 4)) - (x << 1); }
static long f977(long x) { return ((x * 7 + 977) ^ (x >> 5)) - (x << 1); }
static long f978(long x) { return ((x * 9 + 978) ^ (x >> 6)) - (x << 1); }
static long f979(long x) { return ((x * 11 + 979) ^ (x >> 7)) - (x << 1); }
static long f980(long x) { return ((x * 3 + 980) ^ (x >> 1)) - (x << 1); }
static long f981(long x) { return ((x * 5 + 981) ^ (x >> 2)) - (x << 1); }
static long f982(long x) { return ((x * 7 + 982) ^ (x >> 3)) - (x << 1); }
static long f983(long x) { return ((x * 9 + 983) ^ (x >> 4)) - (x << 1); }
static long f984(long x) { return ((x * 11 + 984) ^ (x >> 5)) - (x << 1); }
static long f985(long x) { return ((x * 3 + 985) ^ (x >> 6)) - (x << 1); }
static long f986(long x) { return ((x * 5 + 986) ^ (x >> 7)) - (x << 1); }
static long f987(long x) { return ((x * 7 + 987) ^ (x >> 1)) - (x << 1); }
static long f988(long x) { return ((x * 9 + 988) ^ (x >> 2)) - (x << 1); }
static long f989(long x) { return ((x * 11 + 989) ^ (x >> 3)) - (x << 1); }
static long f990(long x) { return ((x * 3 + 990) ^ (x >> 4)) - (x << 1); }
static long f991(long x) { return ((x * 5 + 991) ^ (x >> 5)) - (x << 1); }
static long f992(long x) { return ((x * 7 + 992) ^ (x >> 6)) - (x << 1); }
static long f993(long x) { return ((x * 9 + 993) ^ (x >> 7)) - (x << 1); }
static long f994(long x) { return ((x * 11 + 994) ^ (x >> 1)) - (x << 1); }
static long f995(long x) { return ((x * 3 + 995) ^ (x >> 2)) - (x << 1); }
static long f996(long x) { return ((x * 5 + 996) ^ (x >> 3)) - (x << 1); }
static long f997(long x) { return ((x * 7 + 997) ^ (x >> 4)) - (x << 1); }
static long f998(long x) { return ((x * 9 + 998) ^ (x >> 5)) - (x << 1); }
static long f999(long x) { return ((x * 11 + 999) ^ (x >> 6)) - (x << 1); }

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
    acc += f55(acc + 55);
    acc += f56(acc + 56);
    acc += f57(acc + 57);
    acc += f58(acc + 58);
    acc += f59(acc + 59);
    acc += f60(acc + 60);
    acc += f61(acc + 61);
    acc += f62(acc + 62);
    acc += f63(acc + 63);
    acc += f64(acc + 64);
    acc += f65(acc + 65);
    acc += f66(acc + 66);
    acc += f67(acc + 67);
    acc += f68(acc + 68);
    acc += f69(acc + 69);
    acc += f70(acc + 70);
    acc += f71(acc + 71);
    acc += f72(acc + 72);
    acc += f73(acc + 73);
    acc += f74(acc + 74);
    acc += f75(acc + 75);
    acc += f76(acc + 76);
    acc += f77(acc + 77);
    acc += f78(acc + 78);
    acc += f79(acc + 79);
    acc += f80(acc + 80);
    acc += f81(acc + 81);
    acc += f82(acc + 82);
    acc += f83(acc + 83);
    acc += f84(acc + 84);
    acc += f85(acc + 85);
    acc += f86(acc + 86);
    acc += f87(acc + 87);
    acc += f88(acc + 88);
    acc += f89(acc + 89);
    acc += f90(acc + 90);
    acc += f91(acc + 91);
    acc += f92(acc + 92);
    acc += f93(acc + 93);
    acc += f94(acc + 94);
    acc += f95(acc + 95);
    acc += f96(acc + 96);
    acc += f97(acc + 97);
    acc += f98(acc + 98);
    acc += f99(acc + 99);
    acc += f100(acc + 100);
    acc += f101(acc + 101);
    acc += f102(acc + 102);
    acc += f103(acc + 103);
    acc += f104(acc + 104);
    acc += f105(acc + 105);
    acc += f106(acc + 106);
    acc += f107(acc + 107);
    acc += f108(acc + 108);
    acc += f109(acc + 109);
    acc += f110(acc + 110);
    acc += f111(acc + 111);
    acc += f112(acc + 112);
    acc += f113(acc + 113);
    acc += f114(acc + 114);
    acc += f115(acc + 115);
    acc += f116(acc + 116);
    acc += f117(acc + 117);
    acc += f118(acc + 118);
    acc += f119(acc + 119);
    acc += f120(acc + 120);
    acc += f121(acc + 121);
    acc += f122(acc + 122);
    acc += f123(acc + 123);
    acc += f124(acc + 124);
    acc += f125(acc + 125);
    acc += f126(acc + 126);
    acc += f127(acc + 127);
    acc += f128(acc + 128);
    acc += f129(acc + 129);
    acc += f130(acc + 130);
    acc += f131(acc + 131);
    acc += f132(acc + 132);
    acc += f133(acc + 133);
    acc += f134(acc + 134);
    acc += f135(acc + 135);
    acc += f136(acc + 136);
    acc += f137(acc + 137);
    acc += f138(acc + 138);
    acc += f139(acc + 139);
    acc += f140(acc + 140);
    acc += f141(acc + 141);
    acc += f142(acc + 142);
    acc += f143(acc + 143);
    acc += f144(acc + 144);
    acc += f145(acc + 145);
    acc += f146(acc + 146);
    acc += f147(acc + 147);
    acc += f148(acc + 148);
    acc += f149(acc + 149);
    acc += f150(acc + 150);
    acc += f151(acc + 151);
    acc += f152(acc + 152);
    acc += f153(acc + 153);
    acc += f154(acc + 154);
    acc += f155(acc + 155);
    acc += f156(acc + 156);
    acc += f157(acc + 157);
    acc += f158(acc + 158);
    acc += f159(acc + 159);
    acc += f160(acc + 160);
    acc += f161(acc + 161);
    acc += f162(acc + 162);
    acc += f163(acc + 163);
    acc += f164(acc + 164);
    acc += f165(acc + 165);
    acc += f166(acc + 166);
    acc += f167(acc + 167);
    acc += f168(acc + 168);
    acc += f169(acc + 169);
    acc += f170(acc + 170);
    acc += f171(acc + 171);
    acc += f172(acc + 172);
    acc += f173(acc + 173);
    acc += f174(acc + 174);
    acc += f175(acc + 175);
    acc += f176(acc + 176);
    acc += f177(acc + 177);
    acc += f178(acc + 178);
    acc += f179(acc + 179);
    acc += f180(acc + 180);
    acc += f181(acc + 181);
    acc += f182(acc + 182);
    acc += f183(acc + 183);
    acc += f184(acc + 184);
    acc += f185(acc + 185);
    acc += f186(acc + 186);
    acc += f187(acc + 187);
    acc += f188(acc + 188);
    acc += f189(acc + 189);
    acc += f190(acc + 190);
    acc += f191(acc + 191);
    acc += f192(acc + 192);
    acc += f193(acc + 193);
    acc += f194(acc + 194);
    acc += f195(acc + 195);
    acc += f196(acc + 196);
    acc += f197(acc + 197);
    acc += f198(acc + 198);
    acc += f199(acc + 199);
    acc += f200(acc + 200);
    acc += f201(acc + 201);
    acc += f202(acc + 202);
    acc += f203(acc + 203);
    acc += f204(acc + 204);
    acc += f205(acc + 205);
    acc += f206(acc + 206);
    acc += f207(acc + 207);
    acc += f208(acc + 208);
    acc += f209(acc + 209);
    acc += f210(acc + 210);
    acc += f211(acc + 211);
    acc += f212(acc + 212);
    acc += f213(acc + 213);
    acc += f214(acc + 214);
    acc += f215(acc + 215);
    acc += f216(acc + 216);
    acc += f217(acc + 217);
    acc += f218(acc + 218);
    acc += f219(acc + 219);
    acc += f220(acc + 220);
    acc += f221(acc + 221);
    acc += f222(acc + 222);
    acc += f223(acc + 223);
    acc += f224(acc + 224);
    acc += f225(acc + 225);
    acc += f226(acc + 226);
    acc += f227(acc + 227);
    acc += f228(acc + 228);
    acc += f229(acc + 229);
    acc += f230(acc + 230);
    acc += f231(acc + 231);
    acc += f232(acc + 232);
    acc += f233(acc + 233);
    acc += f234(acc + 234);
    acc += f235(acc + 235);
    acc += f236(acc + 236);
    acc += f237(acc + 237);
    acc += f238(acc + 238);
    acc += f239(acc + 239);
    acc += f240(acc + 240);
    acc += f241(acc + 241);
    acc += f242(acc + 242);
    acc += f243(acc + 243);
    acc += f244(acc + 244);
    acc += f245(acc + 245);
    acc += f246(acc + 246);
    acc += f247(acc + 247);
    acc += f248(acc + 248);
    acc += f249(acc + 249);
    acc += f250(acc + 250);
    acc += f251(acc + 251);
    acc += f252(acc + 252);
    acc += f253(acc + 253);
    acc += f254(acc + 254);
    acc += f255(acc + 255);
    acc += f256(acc + 256);
    acc += f257(acc + 257);
    acc += f258(acc + 258);
    acc += f259(acc + 259);
    acc += f260(acc + 260);
    acc += f261(acc + 261);
    acc += f262(acc + 262);
    acc += f263(acc + 263);
    acc += f264(acc + 264);
    acc += f265(acc + 265);
    acc += f266(acc + 266);
    acc += f267(acc + 267);
    acc += f268(acc + 268);
    acc += f269(acc + 269);
    acc += f270(acc + 270);
    acc += f271(acc + 271);
    acc += f272(acc + 272);
    acc += f273(acc + 273);
    acc += f274(acc + 274);
    acc += f275(acc + 275);
    acc += f276(acc + 276);
    acc += f277(acc + 277);
    acc += f278(acc + 278);
    acc += f279(acc + 279);
    acc += f280(acc + 280);
    acc += f281(acc + 281);
    acc += f282(acc + 282);
    acc += f283(acc + 283);
    acc += f284(acc + 284);
    acc += f285(acc + 285);
    acc += f286(acc + 286);
    acc += f287(acc + 287);
    acc += f288(acc + 288);
    acc += f289(acc + 289);
    acc += f290(acc + 290);
    acc += f291(acc + 291);
    acc += f292(acc + 292);
    acc += f293(acc + 293);
    acc += f294(acc + 294);
    acc += f295(acc + 295);
    acc += f296(acc + 296);
    acc += f297(acc + 297);
    acc += f298(acc + 298);
    acc += f299(acc + 299);
    acc += f300(acc + 300);
    acc += f301(acc + 301);
    acc += f302(acc + 302);
    acc += f303(acc + 303);
    acc += f304(acc + 304);
    acc += f305(acc + 305);
    acc += f306(acc + 306);
    acc += f307(acc + 307);
    acc += f308(acc + 308);
    acc += f309(acc + 309);
    acc += f310(acc + 310);
    acc += f311(acc + 311);
    acc += f312(acc + 312);
    acc += f313(acc + 313);
    acc += f314(acc + 314);
    acc += f315(acc + 315);
    acc += f316(acc + 316);
    acc += f317(acc + 317);
    acc += f318(acc + 318);
    acc += f319(acc + 319);
    acc += f320(acc + 320);
    acc += f321(acc + 321);
    acc += f322(acc + 322);
    acc += f323(acc + 323);
    acc += f324(acc + 324);
    acc += f325(acc + 325);
    acc += f326(acc + 326);
    acc += f327(acc + 327);
    acc += f328(acc + 328);
    acc += f329(acc + 329);
    acc += f330(acc + 330);
    acc += f331(acc + 331);
    acc += f332(acc + 332);
    acc += f333(acc + 333);
    acc += f334(acc + 334);
    acc += f335(acc + 335);
    acc += f336(acc + 336);
    acc += f337(acc + 337);
    acc += f338(acc + 338);
    acc += f339(acc + 339);
    acc += f340(acc + 340);
    acc += f341(acc + 341);
    acc += f342(acc + 342);
    acc += f343(acc + 343);
    acc += f344(acc + 344);
    acc += f345(acc + 345);
    acc += f346(acc + 346);
    acc += f347(acc + 347);
    acc += f348(acc + 348);
    acc += f349(acc + 349);
    acc += f350(acc + 350);
    acc += f351(acc + 351);
    acc += f352(acc + 352);
    acc += f353(acc + 353);
    acc += f354(acc + 354);
    acc += f355(acc + 355);
    acc += f356(acc + 356);
    acc += f357(acc + 357);
    acc += f358(acc + 358);
    acc += f359(acc + 359);
    acc += f360(acc + 360);
    acc += f361(acc + 361);
    acc += f362(acc + 362);
    acc += f363(acc + 363);
    acc += f364(acc + 364);
    acc += f365(acc + 365);
    acc += f366(acc + 366);
    acc += f367(acc + 367);
    acc += f368(acc + 368);
    acc += f369(acc + 369);
    acc += f370(acc + 370);
    acc += f371(acc + 371);
    acc += f372(acc + 372);
    acc += f373(acc + 373);
    acc += f374(acc + 374);
    acc += f375(acc + 375);
    acc += f376(acc + 376);
    acc += f377(acc + 377);
    acc += f378(acc + 378);
    acc += f379(acc + 379);
    acc += f380(acc + 380);
    acc += f381(acc + 381);
    acc += f382(acc + 382);
    acc += f383(acc + 383);
    acc += f384(acc + 384);
    acc += f385(acc + 385);
    acc += f386(acc + 386);
    acc += f387(acc + 387);
    acc += f388(acc + 388);
    acc += f389(acc + 389);
    acc += f390(acc + 390);
    acc += f391(acc + 391);
    acc += f392(acc + 392);
    acc += f393(acc + 393);
    acc += f394(acc + 394);
    acc += f395(acc + 395);
    acc += f396(acc + 396);
    acc += f397(acc + 397);
    acc += f398(acc + 398);
    acc += f399(acc + 399);
    acc += f400(acc + 400);
    acc += f401(acc + 401);
    acc += f402(acc + 402);
    acc += f403(acc + 403);
    acc += f404(acc + 404);
    acc += f405(acc + 405);
    acc += f406(acc + 406);
    acc += f407(acc + 407);
    acc += f408(acc + 408);
    acc += f409(acc + 409);
    acc += f410(acc + 410);
    acc += f411(acc + 411);
    acc += f412(acc + 412);
    acc += f413(acc + 413);
    acc += f414(acc + 414);
    acc += f415(acc + 415);
    acc += f416(acc + 416);
    acc += f417(acc + 417);
    acc += f418(acc + 418);
    acc += f419(acc + 419);
    acc += f420(acc + 420);
    acc += f421(acc + 421);
    acc += f422(acc + 422);
    acc += f423(acc + 423);
    acc += f424(acc + 424);
    acc += f425(acc + 425);
    acc += f426(acc + 426);
    acc += f427(acc + 427);
    acc += f428(acc + 428);
    acc += f429(acc + 429);
    acc += f430(acc + 430);
    acc += f431(acc + 431);
    acc += f432(acc + 432);
    acc += f433(acc + 433);
    acc += f434(acc + 434);
    acc += f435(acc + 435);
    acc += f436(acc + 436);
    acc += f437(acc + 437);
    acc += f438(acc + 438);
    acc += f439(acc + 439);
    acc += f440(acc + 440);
    acc += f441(acc + 441);
    acc += f442(acc + 442);
    acc += f443(acc + 443);
    acc += f444(acc + 444);
    acc += f445(acc + 445);
    acc += f446(acc + 446);
    acc += f447(acc + 447);
    acc += f448(acc + 448);
    acc += f449(acc + 449);
    acc += f450(acc + 450);
    acc += f451(acc + 451);
    acc += f452(acc + 452);
    acc += f453(acc + 453);
    acc += f454(acc + 454);
    acc += f455(acc + 455);
    acc += f456(acc + 456);
    acc += f457(acc + 457);
    acc += f458(acc + 458);
    acc += f459(acc + 459);
    acc += f460(acc + 460);
    acc += f461(acc + 461);
    acc += f462(acc + 462);
    acc += f463(acc + 463);
    acc += f464(acc + 464);
    acc += f465(acc + 465);
    acc += f466(acc + 466);
    acc += f467(acc + 467);
    acc += f468(acc + 468);
    acc += f469(acc + 469);
    acc += f470(acc + 470);
    acc += f471(acc + 471);
    acc += f472(acc + 472);
    acc += f473(acc + 473);
    acc += f474(acc + 474);
    acc += f475(acc + 475);
    acc += f476(acc + 476);
    acc += f477(acc + 477);
    acc += f478(acc + 478);
    acc += f479(acc + 479);
    acc += f480(acc + 480);
    acc += f481(acc + 481);
    acc += f482(acc + 482);
    acc += f483(acc + 483);
    acc += f484(acc + 484);
    acc += f485(acc + 485);
    acc += f486(acc + 486);
    acc += f487(acc + 487);
    acc += f488(acc + 488);
    acc += f489(acc + 489);
    acc += f490(acc + 490);
    acc += f491(acc + 491);
    acc += f492(acc + 492);
    acc += f493(acc + 493);
    acc += f494(acc + 494);
    acc += f495(acc + 495);
    acc += f496(acc + 496);
    acc += f497(acc + 497);
    acc += f498(acc + 498);
    acc += f499(acc + 499);
    acc += f500(acc + 500);
    acc += f501(acc + 501);
    acc += f502(acc + 502);
    acc += f503(acc + 503);
    acc += f504(acc + 504);
    acc += f505(acc + 505);
    acc += f506(acc + 506);
    acc += f507(acc + 507);
    acc += f508(acc + 508);
    acc += f509(acc + 509);
    acc += f510(acc + 510);
    acc += f511(acc + 511);
    acc += f512(acc + 512);
    acc += f513(acc + 513);
    acc += f514(acc + 514);
    acc += f515(acc + 515);
    acc += f516(acc + 516);
    acc += f517(acc + 517);
    acc += f518(acc + 518);
    acc += f519(acc + 519);
    acc += f520(acc + 520);
    acc += f521(acc + 521);
    acc += f522(acc + 522);
    acc += f523(acc + 523);
    acc += f524(acc + 524);
    acc += f525(acc + 525);
    acc += f526(acc + 526);
    acc += f527(acc + 527);
    acc += f528(acc + 528);
    acc += f529(acc + 529);
    acc += f530(acc + 530);
    acc += f531(acc + 531);
    acc += f532(acc + 532);
    acc += f533(acc + 533);
    acc += f534(acc + 534);
    acc += f535(acc + 535);
    acc += f536(acc + 536);
    acc += f537(acc + 537);
    acc += f538(acc + 538);
    acc += f539(acc + 539);
    acc += f540(acc + 540);
    acc += f541(acc + 541);
    acc += f542(acc + 542);
    acc += f543(acc + 543);
    acc += f544(acc + 544);
    acc += f545(acc + 545);
    acc += f546(acc + 546);
    acc += f547(acc + 547);
    acc += f548(acc + 548);
    acc += f549(acc + 549);
    acc += f550(acc + 550);
    acc += f551(acc + 551);
    acc += f552(acc + 552);
    acc += f553(acc + 553);
    acc += f554(acc + 554);
    acc += f555(acc + 555);
    acc += f556(acc + 556);
    acc += f557(acc + 557);
    acc += f558(acc + 558);
    acc += f559(acc + 559);
    acc += f560(acc + 560);
    acc += f561(acc + 561);
    acc += f562(acc + 562);
    acc += f563(acc + 563);
    acc += f564(acc + 564);
    acc += f565(acc + 565);
    acc += f566(acc + 566);
    acc += f567(acc + 567);
    acc += f568(acc + 568);
    acc += f569(acc + 569);
    acc += f570(acc + 570);
    acc += f571(acc + 571);
    acc += f572(acc + 572);
    acc += f573(acc + 573);
    acc += f574(acc + 574);
    acc += f575(acc + 575);
    acc += f576(acc + 576);
    acc += f577(acc + 577);
    acc += f578(acc + 578);
    acc += f579(acc + 579);
    acc += f580(acc + 580);
    acc += f581(acc + 581);
    acc += f582(acc + 582);
    acc += f583(acc + 583);
    acc += f584(acc + 584);
    acc += f585(acc + 585);
    acc += f586(acc + 586);
    acc += f587(acc + 587);
    acc += f588(acc + 588);
    acc += f589(acc + 589);
    acc += f590(acc + 590);
    acc += f591(acc + 591);
    acc += f592(acc + 592);
    acc += f593(acc + 593);
    acc += f594(acc + 594);
    acc += f595(acc + 595);
    acc += f596(acc + 596);
    acc += f597(acc + 597);
    acc += f598(acc + 598);
    acc += f599(acc + 599);
    acc += f600(acc + 600);
    acc += f601(acc + 601);
    acc += f602(acc + 602);
    acc += f603(acc + 603);
    acc += f604(acc + 604);
    acc += f605(acc + 605);
    acc += f606(acc + 606);
    acc += f607(acc + 607);
    acc += f608(acc + 608);
    acc += f609(acc + 609);
    acc += f610(acc + 610);
    acc += f611(acc + 611);
    acc += f612(acc + 612);
    acc += f613(acc + 613);
    acc += f614(acc + 614);
    acc += f615(acc + 615);
    acc += f616(acc + 616);
    acc += f617(acc + 617);
    acc += f618(acc + 618);
    acc += f619(acc + 619);
    acc += f620(acc + 620);
    acc += f621(acc + 621);
    acc += f622(acc + 622);
    acc += f623(acc + 623);
    acc += f624(acc + 624);
    acc += f625(acc + 625);
    acc += f626(acc + 626);
    acc += f627(acc + 627);
    acc += f628(acc + 628);
    acc += f629(acc + 629);
    acc += f630(acc + 630);
    acc += f631(acc + 631);
    acc += f632(acc + 632);
    acc += f633(acc + 633);
    acc += f634(acc + 634);
    acc += f635(acc + 635);
    acc += f636(acc + 636);
    acc += f637(acc + 637);
    acc += f638(acc + 638);
    acc += f639(acc + 639);
    acc += f640(acc + 640);
    acc += f641(acc + 641);
    acc += f642(acc + 642);
    acc += f643(acc + 643);
    acc += f644(acc + 644);
    acc += f645(acc + 645);
    acc += f646(acc + 646);
    acc += f647(acc + 647);
    acc += f648(acc + 648);
    acc += f649(acc + 649);
    acc += f650(acc + 650);
    acc += f651(acc + 651);
    acc += f652(acc + 652);
    acc += f653(acc + 653);
    acc += f654(acc + 654);
    acc += f655(acc + 655);
    acc += f656(acc + 656);
    acc += f657(acc + 657);
    acc += f658(acc + 658);
    acc += f659(acc + 659);
    acc += f660(acc + 660);
    acc += f661(acc + 661);
    acc += f662(acc + 662);
    acc += f663(acc + 663);
    acc += f664(acc + 664);
    acc += f665(acc + 665);
    acc += f666(acc + 666);
    acc += f667(acc + 667);
    acc += f668(acc + 668);
    acc += f669(acc + 669);
    acc += f670(acc + 670);
    acc += f671(acc + 671);
    acc += f672(acc + 672);
    acc += f673(acc + 673);
    acc += f674(acc + 674);
    acc += f675(acc + 675);
    acc += f676(acc + 676);
    acc += f677(acc + 677);
    acc += f678(acc + 678);
    acc += f679(acc + 679);
    acc += f680(acc + 680);
    acc += f681(acc + 681);
    acc += f682(acc + 682);
    acc += f683(acc + 683);
    acc += f684(acc + 684);
    acc += f685(acc + 685);
    acc += f686(acc + 686);
    acc += f687(acc + 687);
    acc += f688(acc + 688);
    acc += f689(acc + 689);
    acc += f690(acc + 690);
    acc += f691(acc + 691);
    acc += f692(acc + 692);
    acc += f693(acc + 693);
    acc += f694(acc + 694);
    acc += f695(acc + 695);
    acc += f696(acc + 696);
    acc += f697(acc + 697);
    acc += f698(acc + 698);
    acc += f699(acc + 699);
    acc += f700(acc + 700);
    acc += f701(acc + 701);
    acc += f702(acc + 702);
    acc += f703(acc + 703);
    acc += f704(acc + 704);
    acc += f705(acc + 705);
    acc += f706(acc + 706);
    acc += f707(acc + 707);
    acc += f708(acc + 708);
    acc += f709(acc + 709);
    acc += f710(acc + 710);
    acc += f711(acc + 711);
    acc += f712(acc + 712);
    acc += f713(acc + 713);
    acc += f714(acc + 714);
    acc += f715(acc + 715);
    acc += f716(acc + 716);
    acc += f717(acc + 717);
    acc += f718(acc + 718);
    acc += f719(acc + 719);
    acc += f720(acc + 720);
    acc += f721(acc + 721);
    acc += f722(acc + 722);
    acc += f723(acc + 723);
    acc += f724(acc + 724);
    acc += f725(acc + 725);
    acc += f726(acc + 726);
    acc += f727(acc + 727);
    acc += f728(acc + 728);
    acc += f729(acc + 729);
    acc += f730(acc + 730);
    acc += f731(acc + 731);
    acc += f732(acc + 732);
    acc += f733(acc + 733);
    acc += f734(acc + 734);
    acc += f735(acc + 735);
    acc += f736(acc + 736);
    acc += f737(acc + 737);
    acc += f738(acc + 738);
    acc += f739(acc + 739);
    acc += f740(acc + 740);
    acc += f741(acc + 741);
    acc += f742(acc + 742);
    acc += f743(acc + 743);
    acc += f744(acc + 744);
    acc += f745(acc + 745);
    acc += f746(acc + 746);
    acc += f747(acc + 747);
    acc += f748(acc + 748);
    acc += f749(acc + 749);
    acc += f750(acc + 750);
    acc += f751(acc + 751);
    acc += f752(acc + 752);
    acc += f753(acc + 753);
    acc += f754(acc + 754);
    acc += f755(acc + 755);
    acc += f756(acc + 756);
    acc += f757(acc + 757);
    acc += f758(acc + 758);
    acc += f759(acc + 759);
    acc += f760(acc + 760);
    acc += f761(acc + 761);
    acc += f762(acc + 762);
    acc += f763(acc + 763);
    acc += f764(acc + 764);
    acc += f765(acc + 765);
    acc += f766(acc + 766);
    acc += f767(acc + 767);
    acc += f768(acc + 768);
    acc += f769(acc + 769);
    acc += f770(acc + 770);
    acc += f771(acc + 771);
    acc += f772(acc + 772);
    acc += f773(acc + 773);
    acc += f774(acc + 774);
    acc += f775(acc + 775);
    acc += f776(acc + 776);
    acc += f777(acc + 777);
    acc += f778(acc + 778);
    acc += f779(acc + 779);
    acc += f780(acc + 780);
    acc += f781(acc + 781);
    acc += f782(acc + 782);
    acc += f783(acc + 783);
    acc += f784(acc + 784);
    acc += f785(acc + 785);
    acc += f786(acc + 786);
    acc += f787(acc + 787);
    acc += f788(acc + 788);
    acc += f789(acc + 789);
    acc += f790(acc + 790);
    acc += f791(acc + 791);
    acc += f792(acc + 792);
    acc += f793(acc + 793);
    acc += f794(acc + 794);
    acc += f795(acc + 795);
    acc += f796(acc + 796);
    acc += f797(acc + 797);
    acc += f798(acc + 798);
    acc += f799(acc + 799);
    acc += f800(acc + 800);
    acc += f801(acc + 801);
    acc += f802(acc + 802);
    acc += f803(acc + 803);
    acc += f804(acc + 804);
    acc += f805(acc + 805);
    acc += f806(acc + 806);
    acc += f807(acc + 807);
    acc += f808(acc + 808);
    acc += f809(acc + 809);
    acc += f810(acc + 810);
    acc += f811(acc + 811);
    acc += f812(acc + 812);
    acc += f813(acc + 813);
    acc += f814(acc + 814);
    acc += f815(acc + 815);
    acc += f816(acc + 816);
    acc += f817(acc + 817);
    acc += f818(acc + 818);
    acc += f819(acc + 819);
    acc += f820(acc + 820);
    acc += f821(acc + 821);
    acc += f822(acc + 822);
    acc += f823(acc + 823);
    acc += f824(acc + 824);
    acc += f825(acc + 825);
    acc += f826(acc + 826);
    acc += f827(acc + 827);
    acc += f828(acc + 828);
    acc += f829(acc + 829);
    acc += f830(acc + 830);
    acc += f831(acc + 831);
    acc += f832(acc + 832);
    acc += f833(acc + 833);
    acc += f834(acc + 834);
    acc += f835(acc + 835);
    acc += f836(acc + 836);
    acc += f837(acc + 837);
    acc += f838(acc + 838);
    acc += f839(acc + 839);
    acc += f840(acc + 840);
    acc += f841(acc + 841);
    acc += f842(acc + 842);
    acc += f843(acc + 843);
    acc += f844(acc + 844);
    acc += f845(acc + 845);
    acc += f846(acc + 846);
    acc += f847(acc + 847);
    acc += f848(acc + 848);
    acc += f849(acc + 849);
    acc += f850(acc + 850);
    acc += f851(acc + 851);
    acc += f852(acc + 852);
    acc += f853(acc + 853);
    acc += f854(acc + 854);
    acc += f855(acc + 855);
    acc += f856(acc + 856);
    acc += f857(acc + 857);
    acc += f858(acc + 858);
    acc += f859(acc + 859);
    acc += f860(acc + 860);
    acc += f861(acc + 861);
    acc += f862(acc + 862);
    acc += f863(acc + 863);
    acc += f864(acc + 864);
    acc += f865(acc + 865);
    acc += f866(acc + 866);
    acc += f867(acc + 867);
    acc += f868(acc + 868);
    acc += f869(acc + 869);
    acc += f870(acc + 870);
    acc += f871(acc + 871);
    acc += f872(acc + 872);
    acc += f873(acc + 873);
    acc += f874(acc + 874);
    acc += f875(acc + 875);
    acc += f876(acc + 876);
    acc += f877(acc + 877);
    acc += f878(acc + 878);
    acc += f879(acc + 879);
    acc += f880(acc + 880);
    acc += f881(acc + 881);
    acc += f882(acc + 882);
    acc += f883(acc + 883);
    acc += f884(acc + 884);
    acc += f885(acc + 885);
    acc += f886(acc + 886);
    acc += f887(acc + 887);
    acc += f888(acc + 888);
    acc += f889(acc + 889);
    acc += f890(acc + 890);
    acc += f891(acc + 891);
    acc += f892(acc + 892);
    acc += f893(acc + 893);
    acc += f894(acc + 894);
    acc += f895(acc + 895);
    acc += f896(acc + 896);
    acc += f897(acc + 897);
    acc += f898(acc + 898);
    acc += f899(acc + 899);
    acc += f900(acc + 900);
    acc += f901(acc + 901);
    acc += f902(acc + 902);
    acc += f903(acc + 903);
    acc += f904(acc + 904);
    acc += f905(acc + 905);
    acc += f906(acc + 906);
    acc += f907(acc + 907);
    acc += f908(acc + 908);
    acc += f909(acc + 909);
    acc += f910(acc + 910);
    acc += f911(acc + 911);
    acc += f912(acc + 912);
    acc += f913(acc + 913);
    acc += f914(acc + 914);
    acc += f915(acc + 915);
    acc += f916(acc + 916);
    acc += f917(acc + 917);
    acc += f918(acc + 918);
    acc += f919(acc + 919);
    acc += f920(acc + 920);
    acc += f921(acc + 921);
    acc += f922(acc + 922);
    acc += f923(acc + 923);
    acc += f924(acc + 924);
    acc += f925(acc + 925);
    acc += f926(acc + 926);
    acc += f927(acc + 927);
    acc += f928(acc + 928);
    acc += f929(acc + 929);
    acc += f930(acc + 930);
    acc += f931(acc + 931);
    acc += f932(acc + 932);
    acc += f933(acc + 933);
    acc += f934(acc + 934);
    acc += f935(acc + 935);
    acc += f936(acc + 936);
    acc += f937(acc + 937);
    acc += f938(acc + 938);
    acc += f939(acc + 939);
    acc += f940(acc + 940);
    acc += f941(acc + 941);
    acc += f942(acc + 942);
    acc += f943(acc + 943);
    acc += f944(acc + 944);
    acc += f945(acc + 945);
    acc += f946(acc + 946);
    acc += f947(acc + 947);
    acc += f948(acc + 948);
    acc += f949(acc + 949);
    acc += f950(acc + 950);
    acc += f951(acc + 951);
    acc += f952(acc + 952);
    acc += f953(acc + 953);
    acc += f954(acc + 954);
    acc += f955(acc + 955);
    acc += f956(acc + 956);
    acc += f957(acc + 957);
    acc += f958(acc + 958);
    acc += f959(acc + 959);
    acc += f960(acc + 960);
    acc += f961(acc + 961);
    acc += f962(acc + 962);
    acc += f963(acc + 963);
    acc += f964(acc + 964);
    acc += f965(acc + 965);
    acc += f966(acc + 966);
    acc += f967(acc + 967);
    acc += f968(acc + 968);
    acc += f969(acc + 969);
    acc += f970(acc + 970);
    acc += f971(acc + 971);
    acc += f972(acc + 972);
    acc += f973(acc + 973);
    acc += f974(acc + 974);
    acc += f975(acc + 975);
    acc += f976(acc + 976);
    acc += f977(acc + 977);
    acc += f978(acc + 978);
    acc += f979(acc + 979);
    acc += f980(acc + 980);
    acc += f981(acc + 981);
    acc += f982(acc + 982);
    acc += f983(acc + 983);
    acc += f984(acc + 984);
    acc += f985(acc + 985);
    acc += f986(acc + 986);
    acc += f987(acc + 987);
    acc += f988(acc + 988);
    acc += f989(acc + 989);
    acc += f990(acc + 990);
    acc += f991(acc + 991);
    acc += f992(acc + 992);
    acc += f993(acc + 993);
    acc += f994(acc + 994);
    acc += f995(acc + 995);
    acc += f996(acc + 996);
    acc += f997(acc + 997);
    acc += f998(acc + 998);
    acc += f999(acc + 999);
    static char msg[] = "bench ok X\n";
    msg[9] = 'A' + (acc & 15);
    sys3(1, 1, (long)msg, sizeof msg - 1);
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
