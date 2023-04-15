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

	cout << "����2048bit����......" << endl;
	a = RandomPrime_ZZ(1024, 1);				// �˴�Ϊ�������ٶȺʹ�ӡ�����࣬����������ָ��Ϊ1024bit���������ֻҪ��19-21�е�1024����Ϊ����ֵ����
	b = RandomPrime_ZZ(1024, 1);
	n = RandomPrime_ZZ(1024, 1);
	cout << "a:  " << a << "\n\n" << "b:  " << b << "\n\n" << "n:  " << n << "\n\n";

	cout << "������:" << endl;
	res = Montgomery_mul_mod(a, b, n);
	if (res == a * b % n)
		cout << "ģ�˽����ȷ�����Ϊ:" << endl << res << "\n\n";
	else
		cout << "�������" << endl;

	res = Montgomery_mod(a, n);
	if (res == a % n)
		cout << "ȡ������ȷ�����Ϊ:" << endl << res << "\n\n";
	else
		cout << "�������" << endl;
	return 0;
}
