#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include"AES.h"


int main()
{
	uint8_t init_key[16] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };
	uint8_t plaintext[16] = { 0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34 };
	uint8_t ciphertext[16];
	uint8_t key[11][16];

	LARGE_INTEGER BeginTime;
	LARGE_INTEGER EndTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	printf("加密明文 : ");
	msg_print(plaintext);
	generate_key(init_key, key);

	// 一般加解密结果
	aes_enc(plaintext, ciphertext, key);
	printf("\n加密结果 : "); msg_print(ciphertext);
	aes_dec(plaintext, ciphertext, key);
	printf("解密结果 : "); msg_print(plaintext);

	// 单表查表优化加解密结果
	aes_enc_with_tbox_v1(plaintext, ciphertext, key);
	printf("\n单表查表优化加密结果 : ");
	msg_print(ciphertext);
	aes_dec_with_tbox_v1(plaintext, ciphertext, key);
	printf("单表查表优化解密结果 : ");
	msg_print(plaintext);

	// 多表查表优化加解密结果
	aes_enc_with_tbox_v2(plaintext, ciphertext, key);
	printf("\n多表查表优化加密结果 : ");
	msg_print(ciphertext);
	aes_dec_with_tbox_v2(plaintext, ciphertext, key);
	printf("多表查表优化解密结果 : ");
	msg_print(plaintext);


	// 一般加密测试
	QueryPerformanceCounter(&BeginTime);
	for (int i = 0; i < 100000; ++i)
		aes_enc(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("\n\n加密100000次完成，运行时间%fs\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);

	// 一般解密测试
	QueryPerformanceCounter(&BeginTime);
	for (int i = 0; i < 100000; ++i)
		aes_dec(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("解密100000次完成，运行时间%fs\n\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);

	// 单表查表优化加密测试
	QueryPerformanceCounter(&BeginTime);
	for (int i = 0; i < 100000; ++i)
		aes_enc_with_tbox_v1(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("单表查表优化加密100000次完成，运行时间%fs\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);

	// 单表查表优化解密测试
	QueryPerformanceCounter(&BeginTime);
	for (int i = 0; i < 100000; ++i)
		aes_dec_with_tbox_v1(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("单表查表优化解密100000次完成，运行时间%fs\n\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);

	// 多表查表优化加密测试
	QueryPerformanceCounter(&BeginTime);
	aes_enc_with_tbox_v2(plaintext, ciphertext, key);
	for (int i = 0; i < 100000; ++i)
		aes_enc_with_tbox_v2(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("多表查表优化加密100000次完成，运行时间%fs\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);

	// 多表查表优化解密测试
	QueryPerformanceCounter(&BeginTime);
	for (int i = 0; i < 100000; ++i)
		aes_dec_with_tbox_v2(plaintext, ciphertext, key);
	QueryPerformanceCounter(&EndTime);
	printf("多表查表优化解密100000次完成，运行时间%fs\n", (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart);


	return 0;
}
