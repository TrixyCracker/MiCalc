//Libraries
#include <stdio.h>
#include <stdlib.h>

#include "lib/cstring/cstring.h"

#define ERROR "[!] "

#define VERSION "2.0.0"

void comestero(void);
void mizip(void);
void dump_parser(void);

//Declaration of functions

void comestero_calculate_key(uint8 _Dest[6], const uint8 _Previous_KEY[6], const uint8 _Block, const char _Type);
void mizip_calculate_key(uint8 _Dest[6], const uint8 _UID[4], const uint8 _Block, const char _Type);

void uid_to_uint8(uint8 _Dest[8], char _UID[9]);
uint8 char_to_uint8(char _c);

void wait_input(void);

char buffer[1024];

//Functions
int main(void)
{

	while (1)
	{
		printf("------------------------------\n");
		printf("MICALC v." VERSION "\n");
		printf("------------------------------\n");
		printf("[1] MiZip keys calculator     \n");
		printf("[2] Comestero keys calculator \n");
		printf("[3] Dump parser               \n");
		printf("[9] Exit                      \n");
		printf("------------------------------\n");

		printf(" => ");
		scanf("%s", buffer);

		int choose = string_to_int(buffer);

		printf("\n\n\n\n");

		if (choose == 1) mizip();
		else if (choose == 2) comestero();
		else if (choose == 3) dump_parser();
		else if (choose == 9) break;	

		printf("\n\n\n\n");
	}

	return 0;
}

void mizip(void) 
{

	uint8 KeyA[5][6] = {
		[0] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5}};
	uint8 KeyB[5][6] = {
		[0] = {0xB4, 0xC1, 0x32, 0x43, 0x9E, 0xEF}};

	printf("Enter the UID of MiZip: [HEX] ");
	scanf("%s", buffer);

	if (string_lenght(buffer) != 8) 
	{
		printf(ERROR "UID must be 8 characters lenght! \n");
		return;
	}

	uint8 UID[4];
	if (!string_to_uint8_array(UID, buffer)) 
	{
		printf(ERROR "Invalid characters into UID! \n");
		return;
	}

	printf("\nMizip keys (UID: 0x");
	for (int i = 0; i < 4; i++)
		printf("%02X", UID[i]);
	printf("):\n");

	for (int i = 0; i < 5; i++)
	{

		mizip_calculate_key(KeyA[i], UID, i, 'A');
		mizip_calculate_key(KeyB[i], UID, i, 'B');

		printf("\t- Block %d:\n", i);

		printf("\t\t> Key A: 0x");
		for (int i2 = 0; i2 < 6; i2++)
			printf("%02X", KeyA[i][i2]);
		printf("\n");

		printf("\t\t> Key B: 0x");
		for (int i2 = 0; i2 < 6; i2++)
			printf("%02X", KeyB[i][i2]);
		printf("\n");
	}

}

void comestero(void)
{

	uint8 comestero_keys_A[16][6] = {
		[0] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5}};
	uint8 comestero_keys_B[16][6];

	//Key operations
	printf("Enter the known key: [HEX] ");
	scanf("%s", buffer);

	if (string_lenght(buffer) != 12) 
	{
		printf(ERROR "Key must be 12 characters lenght!");
		return;
	}

	uint8 known_key[6];
	string_to_uint8_array(known_key, buffer);

	//block operations
	printf("Enter the block of the known key: [HEX] ");
	scanf("%s", buffer);

	int known_key_block = string_to_int(buffer);

	if (known_key_block < 0 || known_key_block > 15) 
	{
		printf(ERROR "Please enter a valid block! \n");
		return;
	}
	//---

	//type operations
	printf("Enter the type of known key: [A/b] ");
	scanf("%s", buffer);

	char known_key_type = char_uppercase(buffer[0]); //know key type ius now uppercase

	if (known_key_type != 'A' && known_key_type != 'B') 
	{
		printf(ERROR "Please enter a valid type!\n");
		return;
	}
	//---

	if (known_key_type == 'A')
	{

		if (known_key_block == 0x0) {
			printf("[!] !\n");
			return;
		}

		memory_copy(comestero_keys_A[known_key_block], known_key, sizeof(known_key));

		comestero_calculate_key(comestero_keys_B[known_key_block], comestero_keys_A[known_key_block], known_key_block, '0');
	}
	else if (known_key_type == 'B')
	{

		memory_copy(comestero_keys_B[known_key_block], known_key, sizeof(known_key));

		if (known_key_block == 0x0)
		{
			comestero_calculate_key(comestero_keys_B[0x1], comestero_keys_B[0x0], 0x1, 'B');
			known_key_block = 0x1;
		}

		comestero_calculate_key(comestero_keys_A[known_key_block], comestero_keys_B[known_key_block], known_key_block, '0');

	}

	int block = known_key_block + 1;
	while (block != known_key_block)
	{

		if (block == 16)
		{
			block = 0;
			continue;
		}

		if (block == 0)
			comestero_calculate_key(comestero_keys_B[block], comestero_keys_B[15], block, 'B');
		else
			comestero_calculate_key(comestero_keys_B[block], comestero_keys_B[block - 1], block, 'B');

		//a
		if (block == 1)
			comestero_calculate_key(comestero_keys_A[block], comestero_keys_A[15], block, 'A');
		else
			comestero_calculate_key(comestero_keys_A[block], comestero_keys_A[block - 1], block, 'A');

		block += 1;
	}

	printf("\n\n");

	printf("Comestero keys (Known key: 0x");
	for (int i = 0; i < 6; ++i) printf("%02X", known_key[i]);
	printf(", Block: %d, Key type: %c): \n", known_key_block, known_key_type);
	for (int block = 0; block < 16; ++block)
	{

		printf("\t- Block %X:\n", block);

		printf("\t\t> Key A: 0x");
		for (int byte = 0; byte < 6; ++byte)
		{
			printf("%02X", comestero_keys_A[block][byte]);
		}
		printf("\n");

		printf("\t\t> Key B: 0x");
		for (int byte = 0; byte < 6; ++byte)
		{
			printf("%02X", comestero_keys_B[block][byte]);
		}
		printf("\n");
	}
}

void comestero_calculate_key(uint8 _Dest[6], const uint8 _Previous_KEY[6], const uint8 _Block, const char _Type)
{

	if (_Block == 0 && _Type == 'A')
		return;

	const uint8 xor_table_A[16][6] = {
		[1] = {0x0E, 0x3F, 0x57, 0xAF, 0x05, 0x4B},
		[2] = {0x03, 0x85, 0x64, 0x9A, 0x45, 0xCE},
		[3] = {0x01, 0x52, 0xD0, 0x81, 0x9A, 0x49},
		[4] = {0x07, 0x8F, 0xA7, 0x6B, 0xF3, 0xA2},
		[5] = {0x01, 0x9A, 0x8E, 0xBC, 0xAB, 0x42},
		[6] = {0x03, 0x7B, 0x17, 0xC0, 0xC6, 0x83},
		[7] = {0x01, 0xAD, 0x83, 0xD5, 0xD3, 0x94},
		[8] = {0x0F, 0x13, 0x71, 0x49, 0x01, 0xEF},
		[9] = {0x01, 0xE5, 0x91, 0x79, 0x75, 0x9A},
		[10] = {0x03, 0x48, 0x2C, 0x13, 0xCC, 0xAD},
		[11] = {0x01, 0x3E, 0x38, 0x19, 0x89, 0x66},
		[12] = {0x07, 0x78, 0x7D, 0x00, 0x82, 0xE8},
		[13] = {0x01, 0x03, 0x0B, 0x2D, 0xC7, 0x91},
		[14] = {0x03, 0xE2, 0x5A, 0x8A, 0xC8, 0x2E},
		[15] = {0x01, 0x32, 0x86, 0xEB, 0x2B, 0xD2}
	};

	const uint8 xor_table_B[16][6] = {
		[0] = {0x0F, 0xC3, 0x7E, 0xD8, 0xFA, 0x5C},
		[1] = {0x01, 0xB2, 0x8E, 0xEC, 0xB4, 0x3B},
		[2] = {0x03, 0x80, 0x91, 0xFF, 0xC7, 0x8B},
		[3] = {0x01, 0x76, 0xAD, 0xC5, 0x43, 0x65},
		[4] = {0x07, 0x47, 0xBB, 0xB7, 0x9B, 0xBB},
		[5] = {0x01, 0xB8, 0x63, 0x13, 0xD9, 0xDC},
		[6] = {0x03, 0xD0, 0x8D, 0xB4, 0x14, 0x6D},
		[7] = {0x01, 0x1F, 0xEF, 0xCC, 0x46, 0xA2},
		[8] = {0x0F, 0x21, 0x9B, 0x74, 0x0F, 0x80},
		[9] = {0x01, 0xE6, 0xA5, 0xD4, 0xDF, 0x7C},
		[10] = {0x03, 0x85, 0x84, 0x60, 0xB7, 0xF8},
		[11] = {0x01, 0x5A, 0xA2, 0xD7, 0x4C, 0x4A},
		[12] = {0x07, 0x31, 0x65, 0x0F, 0x62, 0x18},
		[13] = {0x01, 0xFF, 0x19, 0xEC, 0x3F, 0x4E},
		[14] = {0x03, 0xCF, 0x7C, 0x1C, 0xA9, 0xE0},
		[15] = {0x01, 0xAE, 0x6E, 0x3A, 0x05, 0xD9}
	};

	const uint8 xor_table_switch[16][6] = {
		[1] = {0x10, 0x7b, 0x94, 0x22, 0x59, 0x24},
		[2] = {0x10, 0x7e, 0x61, 0x47, 0xdb, 0x61},
		[3] = {0x10, 0x5a, 0x1c, 0x03, 0x02, 0x4d},
		[4] = {0x10, 0x92, 0x00, 0xdf, 0x6a, 0x54},
		[5] = {0x10, 0xb0, 0xed, 0x70, 0x18, 0xca},
		[6] = {0x10, 0x1b, 0x77, 0x04, 0xca, 0x24},
		[7] = {0x10, 0xa9, 0x1b, 0x1d, 0x5f, 0x12},
		[8] = {0x10, 0x9b, 0xf1, 0x20, 0x51, 0x7d},
		[9] = {0x10, 0x98, 0xc5, 0x8d, 0xfb, 0x9b},
		[10] = {0x10, 0x55, 0x6d, 0xfe, 0x80, 0xce},
		[11] = {0x10, 0x31, 0xf7, 0x30, 0x45, 0xe2},
		[12] = {0x10, 0x78, 0xef, 0x3f, 0xa5, 0x12},
		[13] = {0x10, 0x84, 0xfd, 0xfe, 0x5d, 0xcd},
		[14] = {0x10, 0xa9, 0xdb, 0x68, 0x3c, 0x03},
		[15] = {0x10, 0x35, 0x33, 0xb9, 0x12, 0x08}
	};

	if (_Type == 'A')
	{

		for (int i = 0; i < 6; i++)
			_Dest[i] = xor_table_A[_Block][i] ^ _Previous_KEY[i];

	}
	else if (_Type == 'B')
	{

		for (int i = 0; i < 6; i++)
			_Dest[i] = xor_table_B[_Block][i] ^ _Previous_KEY[i];

	}
	else if (_Type == '0')
	{

		for (int i = 0; i < 6; i++)
			_Dest[i] = xor_table_switch[_Block][i] ^ _Previous_KEY[i];

	}

}

void mizip_calculate_key(uint8 _Dest[6], const uint8 _UID[4], const uint8 _Block, const char _Type)
{

	//Key of block 0 is already defined
	if (_Block == 0)
		return;

	const uint8 byte_rules_keyA[6] = {0, 1, 2, 3, 0, 1};

	const uint8 byte_rules_keyB[6] = {2, 3, 0, 1, 2, 3};

	const uint8 xor_table_keyA[5][6] = {
		[1] = {0x09, 0x12, 0x5A, 0x25, 0x89, 0xE5},
		[2] = {0xAB, 0x75, 0xC9, 0x37, 0x92, 0x2F},
		[3] = {0xE2, 0x72, 0x41, 0xAF, 0x2C, 0x09},
		[4] = {0x31, 0x7A, 0xB7, 0x2F, 0x44, 0x90}};

	const uint8 xor_table_keyB[5][6] = {
		[1] = {0xF1, 0x2C, 0x84, 0x53, 0xD8, 0x21},
		[2] = {0x73, 0xE7, 0x99, 0xFE, 0x32, 0x41},
		[3] = {0xAA, 0x4D, 0x13, 0x76, 0x56, 0xAE},
		[4] = {0xB0, 0x13, 0x27, 0x27, 0x2D, 0xFD}};

	for (int i = 0; i < 6; i++)
	{
		if (_Type == 'A')
			_Dest[i] = _UID[byte_rules_keyA[i]] ^ xor_table_keyA[_Block][i];
		else if (_Type == 'B')
			_Dest[i] = _UID[byte_rules_keyB[i]] ^ xor_table_keyB[_Block][i];
	}

}
 void dump_parser(void) {
	 return;
 }