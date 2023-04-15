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
    cout << "生成1024bit素数......" << endl;
    p = RandomPrime_ZZ(256, 1);             // 此处为了生成速度和打印结果简洁，将生成素数指定为256bit，若需更改只要将21-22行的256更改为所需值即可
    q = RandomPrime_ZZ(256, 1);
    n = p * q;

    cout << "p:  " << p << "\n\n" << "q:  " << q << "\n\n" << "n:  " << n << "\n\n";
    cout << "生成密钥......" << endl;
    keypair = RSA_keygen(p, q, n);
    d = keypair.first; e = keypair.second;
    cout << "d:  " << d << "\n\n" << "e:  " << e << "\n\n";
    ciphertext = RSA_enc(m, e, n);
    cout <<"加密明文: "<<m<<"\n\n" << "加密结果:\n" << ciphertext << "\n\n";

    plaintext = RSA_CRT_dec(ciphertext, d, p, q, n);
    if (plaintext == m)
        cout << "解密结果: " << plaintext<< "\n\n";
    else
        cout << "解密失败" << endl;

    plaintext = RSA_dec(ciphertext, d, n);
    if (plaintext == m)
        cout << "CRT解密结果: " << plaintext<< endl;
    else
        cout << "解密失败" << endl;
    return    0;
}
