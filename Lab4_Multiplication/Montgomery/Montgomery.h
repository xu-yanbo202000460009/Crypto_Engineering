#pragma once
#include"NTL/ZZ.h"
#include<stdint.h>

using namespace std;
using namespace NTL;

#define MAX(a,b) (a)>(b)?(a):(b)

uint32_t cal_bitlen(ZZ m)                       // ����m�����Ƴ���
{
    uint32_t res = 0;
    while (m > (ZZ)0)
    {
        m = m >> 1;
        ++res;
    }
    return res;
}


ZZ reduce(ZZ t, ZZ r, ZZ n_inv, ZZ n)           // Montgomeryת��
{
    uint32_t bitlen;
    ZZ mod_mask, m, res;

    if (t<0 || t>r * n - 1)                     // t����[0,n-1]��������Ҫ�����r
        return (ZZ)0;

    mod_mask = r - 1;                           // ģrȡ������Ӧ�Ķ��������룬����ģ8(1000)��������(111)
    m = ((t & mod_mask) * n_inv) & mod_mask;    // m = ((T mod R)N') mod R ȡģ��ת���������

    bitlen = cal_bitlen(r) - 1;                 // ��8(1000)����������3λ
    t = (t + m * n) >> bitlen;                  // (T+mN)/R ������ת������λ����

    if (t < n)
        res = t;
    else
        res = t - n;

    if (res < 0 or res > n)                     // ת����������n��ʣ��ϵ��
        return (ZZ)0;
    else
        return res;
}


ZZ Montgomery_mod(ZZ a, ZZ n)                   // Montgomery�㷨ȡ��
{
    uint32_t bitlen, explen;
    ZZ r, r1, x, y, gcd, n_inv;

    bitlen = MAX(cal_bitlen(a), cal_bitlen(n)); // ת�������ģʽӦΪa��n�����ģ�Ҳ����ֱ����n�Ķ����Ƴ���
    explen = (bitlen + 7) / 8 * 8;              // ��ģ���Ķ�����λ��

    r = (ZZ)2 << explen;                        // r = 2 ** exp_len��Ϊ��ģ��
    r1 = r % n;                                 // ��Ȼ�Ƕ�nȡģ�����ǿ���Ԥ����

    XGCD(gcd, x, y, r, n);                      // ����ģn����Ԫ

    if (gcd != 1)                               // ����rһ��Ϊż��(2^k)����nһ��������(����)����˶���һ������
        return (ZZ)0;

    n_inv = (-y) % r;                           // n����Ԫ���෴��

    return reduce(a * r1, r, n_inv, n);         // a mod n = a * r1 * r^(-1) mod n = a*r*r(-1) mod n = a mod n
}


ZZ Montgomery_mul_mod(ZZ a, ZZ b, ZZ n)         // Montgomeryģ���㷨
{
    uint32_t bitlen, explen;
    ZZ r, r2, x, y, gcd, n_inv, aR, bR, abR;

    bitlen = MAX(MAX(cal_bitlen(a), cal_bitlen(b)), cal_bitlen(n));     // ת�������ģʽӦΪa��b��n����������
    explen = (bitlen + 7) / 8 * 8;                                      // ��ģ���Ķ�����λ��

    r = (ZZ)2 << explen;                                                // r = 2 ** exp_len��Ϊ��ģ��
    r2 = r * r % n;                     // ��ȻҲ��һ��ģ�ˣ���������ģ��nȷ����bit_lenҲȷ���ˡ����rҲ��ȷ����
                                        // ʵ�������ֵ����Ԥ���㣬�ں�������ģn�ĳ˷�ʱ�����ٴμ��㣬�ٶȻ�����
    XGCD(gcd, x, y, r, n);                      // ����ģn����Ԫ

    if (gcd != 1)                               // ����rһ��Ϊż��(2^k)����nһ��������(����)����˶���һ������
        return (ZZ)0;

    n_inv = (-y) % r;                           // n����Ԫ���෴��

    aR = reduce(a * r2, r, n_inv, n);           // aR = a * r2 * r^(-1) mod n = a*r^2*r(-1) mod n = a*r mod n
    bR = reduce(b * r2, r, n_inv, n);           // bR = b * r2 * r^(-1) mod n = b*r^2*r(-1) mod n = b*r mod n
    abR = reduce(aR * bR, r, n_inv, n);         // abR = aR*bR * r^(-1) mod n = ab*r^2*r(-1) mod n = ab*r mod n
    return reduce(abR, r, n_inv, n);            // ab = abR * r^(-1) mod n = ab*r*r(-1) mod n = ab mod n
}