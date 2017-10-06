#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define INIT_PERMUT_T_SIZE 	64
#define KEY_PERMUT_T_SIZE 	56
#define SBOX_COLUMNS		16
#define SBOX_ROWS			4
#define SBOX_COUNT			8
#define SBOX_BIT_MASK		0x3f

#define SBOX_INDEX(x,y) x*SBOX_COLUMNS+y

uint8_t init_permut_tbl[] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

uint8_t key_permut_tbl[] = {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3,
	60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
	29, 21, 13, 5, 28, 20, 12, 4
};

uint8_t key_enc_shift_tbl[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

/*TODO:prepare correct shift table for keys*/
uint8_t key_dec_shift_tbl[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};


uint8_t cmpr_permut_tbl[] = {
	14,  17,  11,  24,  1,  5,  3,  28,  
	15,  6,  21,  10, 23,  19 ,  12,  
	4,  26,  8,  16,  7,  27,  20,  13,  
	2, 41,  52,  31, 37,  47,  55,  30,  
	40,  51,  45,  33,  48, 44,  49,  39,  
	56,	34,  53,  46,  42,  50,  36,  29,  32  
};

uint8_t expn_permut_tbl[] = {
	32,  1,  2,  3,  4,  5,  4,  5,  
	6,  7,  8,  9,  8, 9,  10,  11,  
	12,  13,  12,  13,  14,  15,  16,  
	17, 16,  17,  18,  19,  20,  21,  
	20,  21,  22,  23,  24,  25, 24,  
	25,  26,  27,  28,  29,  28,  29,  
	30,  31,  32,  1  
};


int S1[] = {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

int S2[] = {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

int S3[] = {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

int S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

int S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

int S6[] = {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

int S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

int S8[] = {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};
			
int* SB_list[] = {S1, S2, S3, S4, S5, S6, S7, S8};
			
uint8_t PBox_permut_tbl[] = {
	16, 7, 20, 21, 29, 12, 28, 
	17, 1, 15, 23, 26, 5, 18, 
	31, 10, 2, 8, 24, 14, 32, 
	27, 3, 9, 19, 13, 30, 6, 
	22, 11, 4, 25
};

uint8_t fin_permut_tbl[] = {
	40,  8,  48,  16,  56,  24,  64,  32,  39,  7,  47,  15,  55,  
	23,  63,  31, 38,  6,  46,  14,  54,  22,  62,  30,  37,  5,  
	45,  13,  53,  21,  61,  29, 36,  4,  44,  12,  52,  20,  60,  
	28,  35,  3,  43,  11,  51,  19,  59,  27, 34,  2,  42,  10,  
	50,  18,  58,  26,  33,  1,  41,  9,  49,  17,  57,  25  
};


uint64_t permut(uint64_t block, uint8_t block_size, uint8_t* tbl, uint32_t tbl_sz)
{
	
	int i = 0;
	uint64_t new_block = 0;
	uint8_t *blk_ptr = (uint8_t*)&block;
	uint8_t *nblk_ptr = (uint8_t*)&new_block;
	uint8_t tmp_mask = 0;

	/*substitute with adresses*/
	for( i = 0; i < tbl_sz; i++){
		tmp_mask = blk_ptr[(int)((tbl[i]-1)/8)];
		tmp_mask >>= ((tbl[i]-1)%8);
		tmp_mask &= 1;
		if(!tmp_mask)
			continue;
		tmp_mask <<= (i%8);
		nblk_ptr[(int)(i/8)] |= tmp_mask;
	}
		

	return new_block;
}

uint8_t sbox_f(uint8_t block, int sbox_table[])
{
	
	uint8_t row, col;
	uint8_t tmp = block & 63;
	
	row = ((tmp >> 4) & 0x2) | (tmp &  0x1);
	col = (31 & tmp) >> 1;
	
	return sbox_table[SBOX_INDEX(row,col)];
}

uint64_t translate_key(uint64_t key)
{
	int i;
	uint64_t tmp_key = key;
	uint64_t new_key = 0;
	
	for( i = 0; i < 64; i++ ){
		tmp_key = key >> 63 - i;
		tmp_key &= 1;
		tmp_key <<= i;
		new_key |= tmp_key;
	}
	
	return new_key;
}

uint64_t scrink_key(uint64_t key, uint8_t* key_shift_table, uint8_t* key_permut_table, uint8_t iter)
{
	/*TODO: implement*/
	return 0xffffffffffffffff;
}

void f(uint64_t* block, uint64_t key, uint8_t* key_shift_table, uint8_t iter)
{
	int i = 0;
	uint32_t* left_halve = (uint32_t*)block+1;
	uint32_t* right_halve = (uint32_t*)block;
	uint64_t right48_halve = 0;
	uint64_t compr_key = 0;
	uint64_t tmpblock = 0;

	
	/*TODO: implement key permutation*/
	compr_key = scrink_key(key, key_shift_table, key_shift_table, iter);
	
	/*expand right halve*/
	right48_halve = 0xffffffffffff & permut(*right_halve, 64, expn_permut_tbl, 48);
	
	right48_halve ^= compr_key;
	
	/*go through s blocks*/
	*right_halve = 0;
	for(i = 0, tmpblock = 0; i < SBOX_COUNT; i++, tmpblock = 0){
		tmpblock = ((uint64_t) SBOX_BIT_MASK << i * 6) & right48_halve;
		tmpblock = (tmpblock >> i * 6);
		tmpblock = sbox_f(tmpblock, SB_list[i]);
		tmpblock = (tmpblock << i * 4);
		*right_halve |= tmpblock;
	}
	
	/*final permutation*/
	*right_halve = permut(*right_halve, 32, fin_permut_tbl, 32);
	
	*right_halve ^= *left_halve;
	
	/*assignn left block to right*/
	*left_halve ^= *right_halve ^= *left_halve ^= *right_halve;

}

int8_t DES_iteration(uint64_t block, uint64_t key, uint8_t* key_shift_table)
{
	
	int i;
	
	/*pre permutation process*/
	block = permut(block, 64, init_permut_tbl, 64);
	
	for (i = 0; i < 16; i++)
		f(&block, key, key_shift_table, i);
		
	/*TODO: make post permutation*/
}

void main(int argc, char** argv)
{
	
	DES_iteration(0xffff, 0, NULL);
	

}