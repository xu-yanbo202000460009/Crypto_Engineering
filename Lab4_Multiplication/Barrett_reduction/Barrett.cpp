#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"NTL/ZZ.h"
#include<vector>
#include<Windows.h>
#include<stdint.h>
#include"Barrett.h"

using namespace std;
using namespace NTL;


int main()
{
    uint32_t m_bitlen;
    Barrett_ctx* ctx;
    ZZ a, b, m, res, new_mod, mod_mask, mu;

    cout << "生成2048bit素数......" << endl;
    a = RandomPrime_ZZ(1024, 1);        // 此处为了生成速度和打印结果简洁，将生成素数指定为1024bit，若需更改只要将20-22行的1024更改为所需值即可
    b = RandomPrime_ZZ(1024, 1);
    m = RandomPrime_ZZ(1024, 1);
    cout << "a:  " << a << "\n\n" << "b:  " << b << "\n\n" << "m:  " << m << "\n\n";


    // 预计算Barrett所需值
    m_bitlen = cal_bitlen(m);
    new_mod = (ZZ)1 << m_bitlen;
    mod_mask = (new_mod << 1) - 1;
    mu = new_mod * new_mod / m;
    ctx = new Barrett_ctx(m_bitlen, mod_mask, mu);

    cout << "计算结果:" << endl;
    res = Barrett(ctx, a * b, m);
    if (res == a * b % m)
        cout << "模乘结果正确，结果为:" << endl << res << "\n\n";
    else
        cout << "计算错误" << endl;
    return 0;
}

