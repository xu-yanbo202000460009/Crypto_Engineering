#pragma once
#include"NTL/ZZ.h"
#include<stdint.h>

using namespace std;
using namespace NTL;

#define MAX(a,b) (a)>(b)?(a):(b)

uint32_t cal_bitlen(ZZ m)                       // 计算m二进制长度
{
    uint32_t res = 0;
    while (m > (ZZ)0)
    {
        m = m >> 1;
        ++res;
    }
    return res;
}


ZZ reduce(ZZ t, ZZ r, ZZ n_inv, ZZ n)           // Montgomery转换
{
    uint32_t bitlen;
    ZZ mod_mask, m, res;

    if (t<0 || t>r * n - 1)                     // t属于[0,n-1]，否则需要更大的r
        return (ZZ)0;

    mod_mask = r - 1;                           // 模r取余数对应的二进制掩码，例如模8(1000)的掩码是(111)
    m = ((t & mod_mask) * n_inv) & mod_mask;    // m = ((T mod R)N') mod R 取模被转化成与操作

    bitlen = cal_bitlen(r) - 1;                 // 除8(1000)等于向右移3位
    t = (t + m * n) >> bitlen;                  // (T+mN)/R 除法被转化成移位操作

    if (t < n)
        res = t;
    else
        res = t - n;

    if (res < 0 or res > n)                     // 转换后结果还在n的剩余系中
        return (ZZ)0;
    else
        return res;
}


ZZ Montgomery_mod(ZZ a, ZZ n)                   // Montgomery算法取余
{
    uint32_t bitlen, explen;
    ZZ r, r1, x, y, gcd, n_inv;

    bitlen = MAX(cal_bitlen(a), cal_bitlen(n)); // 转换后的新模式应为a、n中最大的，也可以直接用n的二进制长度
    explen = (bitlen + 7) / 8 * 8;              // 新模数的二进制位数

    r = (ZZ)2 << explen;                        // r = 2 ** exp_len作为新模数
    r1 = r % n;                                 // 虽然是对n取模，但是可以预计算

    XGCD(gcd, x, y, r, n);                      // 计算模n的逆元

    if (gcd != 1)                               // 由于r一定为偶数(2^k)，而n一定是奇数(素数)，因此二者一定互素
        return (ZZ)0;

    n_inv = (-y) % r;                           // n的逆元的相反数

    return reduce(a * r1, r, n_inv, n);         // a mod n = a * r1 * r^(-1) mod n = a*r*r(-1) mod n = a mod n
}


ZZ Montgomery_mul_mod(ZZ a, ZZ b, ZZ n)         // Montgomery模乘算法
{
    uint32_t bitlen, explen;
    ZZ r, r2, x, y, gcd, n_inv, aR, bR, abR;

    bitlen = MAX(MAX(cal_bitlen(a), cal_bitlen(b)), cal_bitlen(n));     // 转换后的新模式应为a、b和n三者中最大的
    explen = (bitlen + 7) / 8 * 8;                                      // 新模数的二进制位数

    r = (ZZ)2 << explen;                                                // r = 2 ** exp_len作为新模数
    r2 = r * r % n;                     // 虽然也是一个模乘，但由于在模数n确定后，bit_len也确定了。因此r也被确定，
                                        // 实际上这个值可以预计算，在后续计算模n的乘法时无需再次计算，速度会提升
    XGCD(gcd, x, y, r, n);                      // 计算模n的逆元

    if (gcd != 1)                               // 由于r一定为偶数(2^k)，而n一定是奇数(素数)，因此二者一定互素
        return (ZZ)0;

    n_inv = (-y) % r;                           // n的逆元的相反数

    aR = reduce(a * r2, r, n_inv, n);           // aR = a * r2 * r^(-1) mod n = a*r^2*r(-1) mod n = a*r mod n
    bR = reduce(b * r2, r, n_inv, n);           // bR = b * r2 * r^(-1) mod n = b*r^2*r(-1) mod n = b*r mod n
    abR = reduce(aR * bR, r, n_inv, n);         // abR = aR*bR * r^(-1) mod n = ab*r^2*r(-1) mod n = ab*r mod n
    return reduce(abR, r, n_inv, n);            // ab = abR * r^(-1) mod n = ab*r*r(-1) mod n = ab mod n
}