#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"NTL/ZZ.h"
#include<vector>
#include<Windows.h>
#include<stdint.h>
#include"tools.h"


using namespace std;
using namespace NTL;


int main()
{
    pair<ZZ, ZZ> keypair;
    ZZ p, q, n, d, e, m, plaintext, ciphertext;

    m = (ZZ)10086;
    cout << "����1024bit����......" << endl;
    p = RandomPrime_ZZ(256, 1);             // �˴�Ϊ�������ٶȺʹ�ӡ�����࣬����������ָ��Ϊ256bit���������ֻҪ��21-22�е�256����Ϊ����ֵ����
    q = RandomPrime_ZZ(256, 1);
    n = p * q;

    cout << "p:  " << p << "\n\n" << "q:  " << q << "\n\n" << "n:  " << n << "\n\n";
    cout << "������Կ......" << endl;
    keypair = RSA_keygen(p, q, n);
    d = keypair.first; e = keypair.second;
    cout << "d:  " << d << "\n\n" << "e:  " << e << "\n\n";
    ciphertext = RSA_enc(m, e, n);
    cout <<"��������: "<<m<<"\n\n" << "���ܽ��:\n" << ciphertext << "\n\n";

    plaintext = RSA_CRT_dec(ciphertext, d, p, q, n);
    if (plaintext == m)
        cout << "���ܽ��: " << plaintext<< "\n\n";
    else
        cout << "����ʧ��" << endl;

    plaintext = RSA_dec(ciphertext, d, n);
    if (plaintext == m)
        cout << "CRT���ܽ��: " << plaintext<< endl;
    else
        cout << "����ʧ��" << endl;
    return    0;
}
