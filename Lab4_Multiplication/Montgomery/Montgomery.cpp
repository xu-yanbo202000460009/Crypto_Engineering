#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"NTL/ZZ.h"
#include<vector>
#include<Windows.h>
#include<stdint.h>
#include"Montgomery.h"


using namespace std;
using namespace NTL;


int main()
{
	ZZ a, b, n, res;

	cout << "生成2048bit素数......" << endl;
	a = RandomPrime_ZZ(1024, 1);				// 此处为了生成速度和打印结果简洁，将生成素数指定为1024bit，若需更改只要将19-21行的1024更改为所需值即可
	b = RandomPrime_ZZ(1024, 1);
	n = RandomPrime_ZZ(1024, 1);
	cout << "a:  " << a << "\n\n" << "b:  " << b << "\n\n" << "n:  " << n << "\n\n";

	cout << "计算结果:" << endl;
	res = Montgomery_mul_mod(a, b, n);
	if (res == a * b % n)
		cout << "模乘结果正确，结果为:" << endl << res << "\n\n";
	else
		cout << "计算错误" << endl;

	res = Montgomery_mod(a, n);
	if (res == a % n)
		cout << "取余结果正确，结果为:" << endl << res << "\n\n";
	else
		cout << "计算错误" << endl;
	return 0;
}
