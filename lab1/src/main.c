#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define INIT_PERMUT_T_SIZE 	64
#define KEY_PERMUT_T_SIZE 	56
uint8_t initial_permutaion[] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

uint8_t key_permut_table[] = {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3,
	60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
	29, 21, 13, 5, 28, 20, 12, 4
};

uint8_t key_enc_shift_table[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

uint8_t key_dec_shift_table[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};



uint8_t sbox_3[][16] = {
	{10, 0,  9,  14, 6, 3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
	{13, 7,  0,  9,  3, 4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1},
	{13, 6,  4,  9,  8, 15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
	{1, 10, 13, 0,  6, 9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12}
};


uint64_t permut(uint64_t block, uint8_t block_size, uint8_t* table, uint32_t table_size)
{
	
	int i = 0;
	uint64_t new_block = 0, temp_block = 0;
	uint64_t block_mask = 0;
	
	for( i = 0; i < table_size; i++){
		temp_block = (block) << block_size - table[i] - 1;
		temp_block &= 0x8000000000000000; 
		temp_block >>= i;
		new_block  |= temp_block;
	}
		
		
	for( i = 0; i < block_size; i++)
		block_mask = (block_mask << 1) | 0x1;
		
	return new_block & block_mask;
}

uint8_t sbox_f(uint8_t block, uint8_t sbox_table[][16])
{
	
	uint8_t row, col;
	uint8_t tmp = block & 63;
	
	row = ((tmp >> 4) & 0x2) | (tmp &  0x1);
	col = (31 & tmp) >> 1;
	
	return sbox_table[row][col];
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
}

void f(uint64_t* block, uint64_t key, uint8_t* key_shift_table, uint8_t iter)
{
	
	uint32_t left_halve = *block >> 32;
	uint32_t right_halve = *block & 0xffffffff;
	uint64_t right48_halve = 0;
	
	key = scrink_key(key, key_shift_table, key_shift_table, iter);
	/*TODO:
	 * 	expand right halce
	 * 	xor with key
	 * 	go through s blocks
	 * 	final permutation
	 * 	assignn left block to right
	 */
	
	
}

int8_t DES_iteration(uint64_t block, uint64_t key, uint8_t* key_shift_table)
{
	
	int i;
	
	/*pre permutation process*/
	block = permut(block, 64, initial_permutaion, 64);
	
	for (i = 0; i < 16; i++)
		f(&block, key, key_shift_table, i);
		
	/*TODO: make post permutation*/
}

void main(int argc, char** argv)
{
	

}