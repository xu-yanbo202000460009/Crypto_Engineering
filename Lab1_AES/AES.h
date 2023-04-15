#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include"parameter.h"

void generate_key(uint8_t init_key[16], uint8_t key[11][16])					// 密钥导出
{
	memcpy(key[0], init_key, 16);
	for (int i = 1; i < 11; ++i)
	{
		//第一列
		key[i][0] = key[i - 1][0] ^ S_Box[key[i - 1][13]] ^ Rcon[i - 1];
		key[i][1] = key[i - 1][1] ^ S_Box[key[i - 1][14]];
		key[i][2] = key[i - 1][2] ^ S_Box[key[i - 1][15]];
		key[i][3] = key[i - 1][3] ^ S_Box[key[i - 1][12]];
		//第二列
		key[i][4] = key[i - 1][4] ^ key[i][0];
		key[i][5] = key[i - 1][5] ^ key[i][1];
		key[i][6] = key[i - 1][6] ^ key[i][2];
		key[i][7] = key[i - 1][7] ^ key[i][3];
		//第三列
		key[i][8] = key[i - 1][8] ^ key[i][4];
		key[i][9] = key[i - 1][9] ^ key[i][5];
		key[i][10] = key[i - 1][10] ^ key[i][6];
		key[i][11] = key[i - 1][11] ^ key[i][7];
		//第四列
		key[i][12] = key[i - 1][12] ^ key[i][8];
		key[i][13] = key[i - 1][13] ^ key[i][9];
		key[i][14] = key[i - 1][14] ^ key[i][10];
		key[i][15] = key[i - 1][15] ^ key[i][11];
	}
}
uint8_t mul2(uint8_t a)		// 列混淆乘法×2
{
	return ((a >> 7) & 0x1 ? (a << 1) ^ 0x1b : a << 1);
}

uint8_t mul3(uint8_t a)		// 列混淆乘法×3
{
	return (mul2(a) ^ a);
}

uint8_t mul4(uint8_t a)		// 列混淆乘法×4
{
	return mul2(mul2(a));
}

uint8_t mul8(uint8_t a)		// 列混淆乘法×8
{
	return mul2(mul2(mul2(a)));
}

uint8_t mul9(uint8_t a)		// 列混淆乘法×9
{
	return (mul8(a) ^ a);
}

uint8_t mulB(uint8_t a)		// 列混淆乘法×0xB
{
	return (mul8(a) ^ mul2(a) ^ a);
}

uint8_t mulD(uint8_t a)		// 列混淆乘法×0xD
{
	return (mul8(a) ^ mul4(a) ^ a);
}

uint8_t mulE(uint8_t a)		// 列混淆乘法×0xE
{
	return (mul8(a) ^ mul4(a) ^ mul2(a));
}

void xor_roundkey(uint8_t input[16], uint8_t key[16])		//异或轮密钥
{
	for (int i = 0; i < 16; ++i)
		input[i] ^= key[i];
}

void shift_row(uint8_t input[16])							// 行移位
{
	uint8_t temp;
	temp = input[1]; input[1] = input[5]; input[5] = input[9]; input[9] = input[13]; input[13] = temp;
	temp = input[2]; input[2] = input[10]; input[10] = temp; temp = input[6]; input[6] = input[14]; input[14] = temp;
	temp = input[3]; input[3] = input[15]; input[15] = input[11]; input[11] = input[7]; input[7] = temp;
}

void inv_shift_row(uint8_t input[16])						// 逆行移位
{
	uint8_t temp;
	temp = input[13]; input[13] = input[9]; input[9] = input[5]; input[5] = input[1]; input[1] = temp;
	temp = input[10]; input[10] = input[2]; input[2] = temp; temp = input[6]; input[6] = input[14]; input[14] = temp;
	temp = input[15]; input[15] = input[3]; input[3] = input[7]; input[7] = input[11]; input[11] = temp;
}

void sbox_sub(uint8_t input[16])							// S盒替换
{
	for (int i = 0; i < 16; ++i)
		input[i] = S_Box[input[i]];
}

void inv_sbox_sub(uint8_t input[16])						// 逆S盒替换
{
	for (int i = 0; i < 16; ++i)
		input[i] = Inv_S_Box[input[i]];
}

void mix_column(uint8_t a[16]) {							// 列混淆
	uint8_t b[16];
	b[0] = mul2(a[0]) ^ mul3(a[1]) ^ a[2] ^ a[3];
	b[1] = mul2(a[1]) ^ mul3(a[2]) ^ a[3] ^ a[0];
	b[2] = mul2(a[2]) ^ mul3(a[3]) ^ a[0] ^ a[1];
	b[3] = mul2(a[3]) ^ mul3(a[0]) ^ a[1] ^ a[2];
	b[4] = mul2(a[4]) ^ mul3(a[5]) ^ a[6] ^ a[7];
	b[5] = mul2(a[5]) ^ mul3(a[6]) ^ a[7] ^ a[4];
	b[6] = mul2(a[6]) ^ mul3(a[7]) ^ a[4] ^ a[5];
	b[7] = mul2(a[7]) ^ mul3(a[4]) ^ a[5] ^ a[6];
	b[8] = mul2(a[8]) ^ mul3(a[9]) ^ a[10] ^ a[11];
	b[9] = mul2(a[9]) ^ mul3(a[10]) ^ a[11] ^ a[8];
	b[10] = mul2(a[10]) ^ mul3(a[11]) ^ a[8] ^ a[9];
	b[11] = mul2(a[11]) ^ mul3(a[8]) ^ a[9] ^ a[10];
	b[12] = mul2(a[12]) ^ mul3(a[13]) ^ a[14] ^ a[15];
	b[13] = mul2(a[13]) ^ mul3(a[14]) ^ a[15] ^ a[12];
	b[14] = mul2(a[14]) ^ mul3(a[15]) ^ a[12] ^ a[13];
	b[15] = mul2(a[15]) ^ mul3(a[12]) ^ a[13] ^ a[14];

	memcpy(a, b, 16);
}

void inv_mix_column(uint8_t a[16])							// 逆列混淆
{
	uint8_t b[16];
	b[0] = mulE(a[0]) ^ mulB(a[1]) ^ mulD(a[2]) ^ mul9(a[3]);
	b[1] = mulE(a[1]) ^ mulB(a[2]) ^ mulD(a[3]) ^ mul9(a[0]);
	b[2] = mulE(a[2]) ^ mulB(a[3]) ^ mulD(a[0]) ^ mul9(a[1]);
	b[3] = mulE(a[3]) ^ mulB(a[0]) ^ mulD(a[1]) ^ mul9(a[2]);
	b[4] = mulE(a[4]) ^ mulB(a[5]) ^ mulD(a[6]) ^ mul9(a[7]);
	b[5] = mulE(a[5]) ^ mulB(a[6]) ^ mulD(a[7]) ^ mul9(a[4]);
	b[6] = mulE(a[6]) ^ mulB(a[7]) ^ mulD(a[4]) ^ mul9(a[5]);
	b[7] = mulE(a[7]) ^ mulB(a[4]) ^ mulD(a[5]) ^ mul9(a[6]);
	b[8] = mulE(a[8]) ^ mulB(a[9]) ^ mulD(a[10]) ^ mul9(a[11]);
	b[9] = mulE(a[9]) ^ mulB(a[10]) ^ mulD(a[11]) ^ mul9(a[8]);
	b[10] = mulE(a[10]) ^ mulB(a[11]) ^ mulD(a[8]) ^ mul9(a[9]);
	b[11] = mulE(a[11]) ^ mulB(a[8]) ^ mulD(a[9]) ^ mul9(a[10]);
	b[12] = mulE(a[12]) ^ mulB(a[13]) ^ mulD(a[14]) ^ mul9(a[15]);
	b[13] = mulE(a[13]) ^ mulB(a[14]) ^ mulD(a[15]) ^ mul9(a[12]);
	b[14] = mulE(a[14]) ^ mulB(a[15]) ^ mulD(a[12]) ^ mul9(a[13]);
	b[15] = mulE(a[15]) ^ mulB(a[12]) ^ mulD(a[13]) ^ mul9(a[14]);

	memcpy(a, b, 16);
}
void aes_enc(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 一般AES加密
{
	xor_roundkey(plaintext, key[0]);
	for (int i = 1; i < 10; ++i)
	{
		sbox_sub(plaintext);
		shift_row(plaintext);
		mix_column(plaintext);
		xor_roundkey(plaintext, key[i]);
	}
	sbox_sub(plaintext);
	shift_row(plaintext);
	xor_roundkey(plaintext, key[10]);
	memcpy(ciphertext, plaintext, 16);
}

void aes_dec(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 一般AES解密
{
	xor_roundkey(ciphertext, key[10]);
	inv_shift_row(ciphertext);
	inv_sbox_sub(ciphertext);
	for (int i = 9; i > 0; --i)
	{
		xor_roundkey(ciphertext, key[i]);
		inv_mix_column(ciphertext);
		inv_shift_row(ciphertext);
		inv_sbox_sub(ciphertext);
	}
	xor_roundkey(ciphertext, key[0]);
	memcpy(plaintext, ciphertext, 16);
}

void enc_round_v1(uint8_t state[16], uint8_t key[16])									// 单表查表优化AES加密1轮
{
	uint8_t res[16];
	uint8_t a, b, c, d;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			a = T_Box0[state[i << 2]][j];
			b = T_Box0[state[((i << 2) + 5) % 16]][(j - 1 + 4) % 4];
			c = T_Box0[state[((i << 2) + 10) % 16]][(j - 2 + 4) % 4];
			d = T_Box0[state[((i << 2) + 15) % 16]][(j - 3 + 4) % 4];
			res[(i << 2) + j] = a ^ b ^ c ^ d;
		}
	}
	xor_roundkey(res, key);
	memcpy(state, res, 16);
}

void enc_round_v2(uint8_t state[16], uint8_t key[16])									// 多表查表优化AES加密1轮
{
	uint8_t res[16];
	uint8_t a, b, c, d;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			a = T_Box0[state[i << 2]][j];
			b = T_Box1[state[((i << 2) + 5) % 16]][j];
			c = T_Box2[state[((i << 2) + 10) % 16]][j];
			d = T_Box3[state[((i << 2) + 15) % 16]][j];
			res[(i << 2) + j] = a ^ b ^ c ^ d;
		}
	}
	xor_roundkey(res, key);
	memcpy(state, res, 16);
}

void aes_enc_with_tbox_v1(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 单表查表优化AES加密
{
	uint8_t state[16];
	memcpy(state, plaintext, 16);
	xor_roundkey(state, key[0]);
	for (int i = 1; i < 10; ++i)
		enc_round_v1(state, key[i]);
	sbox_sub(state);
	shift_row(state);
	xor_roundkey(state, key[10]);
	memcpy(ciphertext,state, 16);
}

void aes_enc_with_tbox_v2(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 多表查表优化AES加密
{
	uint8_t state[16];
	memcpy(state, plaintext, 16);
	xor_roundkey(state, key[0]);
	for (int i = 1; i < 10; ++i)
		enc_round_v2(state, key[i]);
	sbox_sub(state);
	shift_row(state);
	xor_roundkey(state, key[10]);
	memcpy(ciphertext, state, 16);
}

void dec_round_v1(uint8_t state[16], uint8_t key[16])												// 单表查表优化AES解密1轮
{
	uint8_t res[16];
	uint8_t a, b, c, d;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			a = Td_Box0[state[i << 2]][j];
			b = Td_Box0[state[((i << 2) + 5) % 16]][(j - 1 + 4) % 4];
			c = Td_Box0[state[((i << 2) + 10) % 16]][(j - 2 + 4) % 4];
			d = Td_Box0[state[((i << 2) + 15) % 16]][(j - 3 + 4) % 4];
			res[(i << 2) + j] = a ^ b ^ c ^ d;
		}
	}
	xor_roundkey(res, key);
	memcpy(state, res, 16);
}

void dec_round_v2(uint8_t state[16], uint8_t key[16])												// 多表查表优化AES解密1轮
{
	uint8_t res[16];
	uint8_t a, b, c, d;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			a = T_Box0[state[i << 2]][j];
			b = T_Box1[state[((i << 2) + 5) % 16]][j];
			c = T_Box2[state[((i << 2) + 10) % 16]][j];
			d = T_Box3[state[((i << 2) + 15) % 16]][j];
			res[(i << 2) + j] = a ^ b ^ c ^ d;
		}
	}
	xor_roundkey(res, key);
	memcpy(state, res, 16);
}

void aes_dec_with_tbox_v1(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 单表查表优化AES解密
{
	uint8_t state[16];
	memcpy(state, plaintext, 16);
	xor_roundkey(state, key[10]);
	for (int i = 9; i > 0; --i)
		dec_round_v1(state, key[i]);
	inv_sbox_sub(state);
	inv_shift_row(state);
	xor_roundkey(state, key[0]);
	memcpy(ciphertext, state, 16);
}

void aes_dec_with_tbox_v2(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t key[11][16])		// 多表查表优化AES解密
{
	uint8_t state[16];
	memcpy(state, plaintext, 16);
	xor_roundkey(state, key[10]);
	for (int i = 9; i > 0; --i)
		dec_round_v2(state, key[i]);
	inv_sbox_sub(state);
	inv_shift_row(state);
	xor_roundkey(state, key[0]);
	memcpy(ciphertext, state, 16);
}

void msg_print(uint8_t msg[16])			// 打印字节序列
{
	for (int i = 0; i < 16; ++i)
		printf("%02x ", msg[i]);
	printf("\n");
}