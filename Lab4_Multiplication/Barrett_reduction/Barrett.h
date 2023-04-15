#pragma once
#include"NTL/ZZ.h"
#include<stdint.h>

using namespace std;
using namespace NTL;


typedef struct Barrett_ctx {        // 预计算Barrett所需值
    uint32_t k;
    ZZ mod_mask, mu;
    Barrett_ctx(uint32_t K=0, ZZ Mod_mask=(ZZ)0, ZZ Mu=(ZZ)0) : k(K), mod_mask(Mod_mask), mu(Mu) {}
}Barrett_ctx;


uint32_t cal_bitlen(ZZ m)           // 计算m二进制长度
{
    uint32_t res = 0;
    while (m > (ZZ)0)
    {
        m = m >> 1;
        ++res;
    }
    return res;
}


ZZ Barrett(Barrett_ctx* ctx,ZZ x, ZZ m)     // Barrett算法
{
    ZZ q1, q2, q3 ,r1, r2, r;

    q1 = x >> (ctx->k - 1);                 // x / 2^(k-1)
    q2 = q1 * ctx->mu;
    q3 = q2 >> (ctx->k + 1);                // q2 / 2^(k+1)

    r1 = x & ctx->mod_mask;                 // x % 2^k
    r2 = (q3 * m) & ctx->mod_mask;          // q3*m % 2^k
    r = r1 - r2;

    if (r < 0)
        r += ctx->mod_mask + 1;             // + 2^(k+1)
    while (r >= m)
        r -= m;

    return r;
}