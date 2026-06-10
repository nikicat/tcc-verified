/* generated benchmark: 240 functions, freestanding (no libc) */
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
    static char msg[] = "bench ok X\n";
    msg[9] = 'A' + (acc & 15);
    sys3(1, 1, (long)msg, sizeof msg - 1);
    sys3(60, 0, 0, 0);
    for (;;)
        ;
}
