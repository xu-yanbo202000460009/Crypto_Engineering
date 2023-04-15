#pragma once
#include"NTL/ZZ.h"
#include<vector>
#include<stdint.h>

using namespace std;
using namespace NTL;

#define MAX(a,b) (a)>(b)?(a):(b)

uint32_t cal_bitlen(ZZ m)                           // 计算m二进制长度
{
    uint32_t res = 0;
    while (m > (ZZ)0)
    {
        m = m >> 1;
        ++res;
    }
    return res;
}


ZZ Barrett(ZZ x, ZZ m)                  // Barrett算法
{
    uint32_t k;
    ZZ q1, q2, q3, r1, r2, r, new_mod, mod_mask, mu;

    // 预计算Barrett所需值
    k = cal_bitlen(m);
    new_mod = (ZZ)1 << k;
    mod_mask = (new_mod << 1) - 1;
    mu = new_mod * new_mod / m;

    q1 = x >> (k - 1);                 // x / 2^(k-1)
    q2 = q1 * mu;
    q3 = q2 >> (k + 1);                // q2 / 2^(k+1)

    r1 = x & mod_mask;                 // x % 2^k
    r2 = (q3 * m) & mod_mask;          // q3 * m % 2^k
    r = r1 - r2;

    if (r < 0)
        r += mod_mask + 1;             // + 2^(k+1)
    while (r >= m)
        r -= m;

    return r;
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

    bitlen = MAX(MAX(cal_bitlen(a), cal_bitlen(b)), cal_bitlen(n));     // 转换后的新模式应为a、b和n三者中最大的，也可以直接用n的二进制长度
    explen = (bitlen + 7) / 8 * 8;                                      // 新模数的二进制位数

    r = (ZZ)2 << explen;                                                // r = 2 ** exp_len作为新模数
    r2 = r * r % n; // 虽然也是一个模乘，但由于在模数n确定后，bit_len也确定了。因此r也被确定，实际上这个值可以进行预计算，这样在后续计算模n的乘法时无需再次计算，速度会大大

    XGCD(gcd, x, y, r, n);                      // 计算模n的逆元

    if (gcd != 1)                               // 由于r一定为偶数(2^k)，而n一定是奇数(素数)，因此二者一定互素
        return (ZZ)0;

    n_inv = (-y) % r;                           // n的逆元的相反数

    aR = reduce(a * r2, r, n_inv, n);           // aR = a * r2 * r^(-1) mod n = a*r^2*r(-1) mod n = a*r mod n
    bR = reduce(b * r2, r, n_inv, n);           // bR = b * r2 * r^(-1) mod n = b*r^2*r(-1) mod n = b*r mod n
    abR = reduce(aR * bR, r, n_inv, n);         // abR = aR*bR * r^(-1) mod n = ab*r^2*r(-1) mod n = ab*r mod n
    return reduce(abR, r, n_inv, n);            // ab = abR * r^(-1) mod n = ab*r*r(-1) mod n = ab mod n
}


ZZ CRT(vector<pair<ZZ, ZZ>> list_rm)                                        // 中国剩余定理
{
    uint32_t size = list_rm.size();
    ZZ res = (ZZ)0, M = (ZZ)1, gcd, s, t;

    for (uint32_t i = 0; i < size; ++i)
    {
        list_rm[i].first = list_rm[i].first % list_rm[i].second;            // 计算 r% m，确保所有的r都在既约系
        M *= list_rm[i].second;                                             // 所有模数的乘积
    }
    for (uint32_t i = 0; i < size; ++i)
    {
        XGCD(gcd, s, t, M / list_rm[i].second, list_rm[i].second);          // 使用拓展Euclid计算的M模m的逆元M'
        res = (res + M / list_rm[i].second * s * list_rm[i].first) % M;     // 计算所有的MM’r并求和
    }
    return res;
}



ZZ FastModExp(ZZ a, ZZ p, ZZ k)                                             // 分治算法计算模乘
{
    if (p == (ZZ)1)
        return Barrett(a, k);                                               // 递归出口为a%k
    else if (p % 2 == (ZZ)1)
        return Barrett(FastModExp(Barrett(a, k), p - 1, k) * a, k);         // 如果是奇数则计算FastModExp(a%k, p - 1, k) * a % k
    else
        return FastModExp(Montgomery_mul_mod(a, a, k), p / 2, k);           // 偶数则计算FastModExp((a*a%k), p // 2, k)
}



ZZ Fast_Power_Mod(ZZ a, ZZ p, ZZ k)                 // 快速幂计算模乘
{
    ZZ res = (ZZ)1;
    while (p > (ZZ)0)
    {
        if (p % 2)
            res = Montgomery_mul_mod(res, a, k);    // 奇数则模乘底数
        a = Montgomery_mul_mod(a, a, k);            // 偶数只平方底数
        p = p >> 1;
    }
    return res % k;
}



pair<ZZ, ZZ> RSA_keygen(ZZ p, ZZ q, ZZ n)           // 密钥生成
{
    ZZ d, e, ruler, s, gcd;
    ruler = (p - 1) * (q - 1);                      // 计算欧拉函数
    e = RandomBnd(n - 1);
    while (GCD(e, ruler) != (ZZ)1)                  // e与欧拉函数互素
        e = RandomBnd(n - 1);
    XGCD(gcd, d, s, e, ruler);                      // 计算e模欧拉函数的逆元
    if (d < 0)                                      // 负数则加模数得到正数
        d += ruler;
    return make_pair(d, e);
}


ZZ RSA_enc(ZZ m, ZZ e, ZZ n)                        // 使用快速模幂求m^e mod n
{
    ZZ res = Fast_Power_Mod(m, e, n);
    return res;
}


ZZ RSA_dec(ZZ c, ZZ d, ZZ n)                        // 使用快速模幂求c^d mod n
{
    ZZ res = Fast_Power_Mod(c, d, n);
    return res;
}


ZZ RSA_CRT_dec(ZZ c, ZZ d, ZZ p, ZZ q, ZZ n)        // CRT加速解密
{
    vector<pair<ZZ, ZZ>> crt;
    ZZ c1, c2, d1, d2, m1, m2, res;
    c1 = Barrett(c, p);                             // c1 = c mod p
    c2 = Barrett(c, q);                             // c2 = c mod q
    d1 = Barrett(d, p - 1);                         // d1 = d mod p-1
    d2 = Barrett(d, q - 1);                         // d2 = d mod q-1
    m1 = Fast_Power_Mod(c1, d1, p);                 // m1 = c1^d1 mod p
    m2 = Fast_Power_Mod(c2, d2, q);                 // m2 = c2^d2 mod q
    crt.push_back(make_pair(m1, p));
    crt.push_back(make_pair(m2, q));
    res = CRT(crt);                                 // CRT求解明文
    return res;
}