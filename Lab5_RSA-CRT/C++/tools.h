#pragma once
#include"NTL/ZZ.h"
#include<vector>
#include<stdint.h>

using namespace std;
using namespace NTL;

#define MAX(a,b) (a)>(b)?(a):(b)

uint32_t cal_bitlen(ZZ m)                           // ����m�����Ƴ���
{
    uint32_t res = 0;
    while (m > (ZZ)0)
    {
        m = m >> 1;
        ++res;
    }
    return res;
}


ZZ Barrett(ZZ x, ZZ m)                  // Barrett�㷨
{
    uint32_t k;
    ZZ q1, q2, q3, r1, r2, r, new_mod, mod_mask, mu;

    // Ԥ����Barrett����ֵ
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

    bitlen = MAX(MAX(cal_bitlen(a), cal_bitlen(b)), cal_bitlen(n));     // ת�������ģʽӦΪa��b��n���������ģ�Ҳ����ֱ����n�Ķ����Ƴ���
    explen = (bitlen + 7) / 8 * 8;                                      // ��ģ���Ķ�����λ��

    r = (ZZ)2 << explen;                                                // r = 2 ** exp_len��Ϊ��ģ��
    r2 = r * r % n; // ��ȻҲ��һ��ģ�ˣ���������ģ��nȷ����bit_lenҲȷ���ˡ����rҲ��ȷ����ʵ�������ֵ���Խ���Ԥ���㣬�����ں�������ģn�ĳ˷�ʱ�����ٴμ��㣬�ٶȻ���

    XGCD(gcd, x, y, r, n);                      // ����ģn����Ԫ

    if (gcd != 1)                               // ����rһ��Ϊż��(2^k)����nһ��������(����)����˶���һ������
        return (ZZ)0;

    n_inv = (-y) % r;                           // n����Ԫ���෴��

    aR = reduce(a * r2, r, n_inv, n);           // aR = a * r2 * r^(-1) mod n = a*r^2*r(-1) mod n = a*r mod n
    bR = reduce(b * r2, r, n_inv, n);           // bR = b * r2 * r^(-1) mod n = b*r^2*r(-1) mod n = b*r mod n
    abR = reduce(aR * bR, r, n_inv, n);         // abR = aR*bR * r^(-1) mod n = ab*r^2*r(-1) mod n = ab*r mod n
    return reduce(abR, r, n_inv, n);            // ab = abR * r^(-1) mod n = ab*r*r(-1) mod n = ab mod n
}


ZZ CRT(vector<pair<ZZ, ZZ>> list_rm)                                        // �й�ʣ�ඨ��
{
    uint32_t size = list_rm.size();
    ZZ res = (ZZ)0, M = (ZZ)1, gcd, s, t;

    for (uint32_t i = 0; i < size; ++i)
    {
        list_rm[i].first = list_rm[i].first % list_rm[i].second;            // ���� r% m��ȷ�����е�r���ڼ�Լϵ
        M *= list_rm[i].second;                                             // ����ģ���ĳ˻�
    }
    for (uint32_t i = 0; i < size; ++i)
    {
        XGCD(gcd, s, t, M / list_rm[i].second, list_rm[i].second);          // ʹ����չEuclid�����Mģm����ԪM'
        res = (res + M / list_rm[i].second * s * list_rm[i].first) % M;     // �������е�MM��r�����
    }
    return res;
}



ZZ FastModExp(ZZ a, ZZ p, ZZ k)                                             // �����㷨����ģ��
{
    if (p == (ZZ)1)
        return Barrett(a, k);                                               // �ݹ����Ϊa%k
    else if (p % 2 == (ZZ)1)
        return Barrett(FastModExp(Barrett(a, k), p - 1, k) * a, k);         // ��������������FastModExp(a%k, p - 1, k) * a % k
    else
        return FastModExp(Montgomery_mul_mod(a, a, k), p / 2, k);           // ż�������FastModExp((a*a%k), p // 2, k)
}



ZZ Fast_Power_Mod(ZZ a, ZZ p, ZZ k)                 // �����ݼ���ģ��
{
    ZZ res = (ZZ)1;
    while (p > (ZZ)0)
    {
        if (p % 2)
            res = Montgomery_mul_mod(res, a, k);    // ������ģ�˵���
        a = Montgomery_mul_mod(a, a, k);            // ż��ֻƽ������
        p = p >> 1;
    }
    return res % k;
}



pair<ZZ, ZZ> RSA_keygen(ZZ p, ZZ q, ZZ n)           // ��Կ����
{
    ZZ d, e, ruler, s, gcd;
    ruler = (p - 1) * (q - 1);                      // ����ŷ������
    e = RandomBnd(n - 1);
    while (GCD(e, ruler) != (ZZ)1)                  // e��ŷ����������
        e = RandomBnd(n - 1);
    XGCD(gcd, d, s, e, ruler);                      // ����eģŷ����������Ԫ
    if (d < 0)                                      // �������ģ���õ�����
        d += ruler;
    return make_pair(d, e);
}


ZZ RSA_enc(ZZ m, ZZ e, ZZ n)                        // ʹ�ÿ���ģ����m^e mod n
{
    ZZ res = Fast_Power_Mod(m, e, n);
    return res;
}


ZZ RSA_dec(ZZ c, ZZ d, ZZ n)                        // ʹ�ÿ���ģ����c^d mod n
{
    ZZ res = Fast_Power_Mod(c, d, n);
    return res;
}


ZZ RSA_CRT_dec(ZZ c, ZZ d, ZZ p, ZZ q, ZZ n)        // CRT���ٽ���
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
    res = CRT(crt);                                 // CRT�������
    return res;
}