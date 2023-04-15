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

    cout << "����2048bit����......" << endl;
    a = RandomPrime_ZZ(1024, 1);        // �˴�Ϊ�������ٶȺʹ�ӡ�����࣬����������ָ��Ϊ1024bit���������ֻҪ��20-22�е�1024����Ϊ����ֵ����
    b = RandomPrime_ZZ(1024, 1);
    m = RandomPrime_ZZ(1024, 1);
    cout << "a:  " << a << "\n\n" << "b:  " << b << "\n\n" << "m:  " << m << "\n\n";


    // Ԥ����Barrett����ֵ
    m_bitlen = cal_bitlen(m);
    new_mod = (ZZ)1 << m_bitlen;
    mod_mask = (new_mod << 1) - 1;
    mu = new_mod * new_mod / m;
    ctx = new Barrett_ctx(m_bitlen, mod_mask, mu);

    cout << "������:" << endl;
    res = Barrett(ctx, a * b, m);
    if (res == a * b % m)
        cout << "ģ�˽����ȷ�����Ϊ:" << endl << res << "\n\n";
    else
        cout << "�������" << endl;
    return 0;
}

