//Libraries
#include <stdio.h>
#include <stdlib.h>

#include "lib/cstring/cstring.h"
#include "tag_utils.c"

#define ERROR "[!] "

#define VERSION "2.0.0 BETA"

void comestero(void);
void mizip(void);
void dump_parser(void);

//Declaration of functions

void comestero_calculate_key(uint8 _Dest[6], const uint8 _Previous_KEY[6], const uint8 _Block, const char _Type);
void mizip_calculate_key(uint8 _Dest[6], const uint8 _UID[4], const uint8 _Block, const char _Type);

void uid_to_uint8(uint8 _Dest[8], char _UID[9]);
uint8 char_to_uint8(char _c);

void wait_input(void);

void sas();

char buffer[1024];

//Functions
int main(void)
{

	sas();

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
	comestero_keys comestero_keys_A;
	comestero_keys comestero_keys_B;

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
			printf(ERROR "Invalid block for calculate keys! \n");
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

	FILE * keys_file;
	keys_file = fopen("comestero", "w");

	for(int a = 0; a < 16; ++a) 
	{
		
		fprintf(keys_file, "%c", '{');
		for (int b = 0; b < 6; ++b)
			fprintf(keys_file, "0x%02X,", comestero_keys_A[a][b]);
		fprintf(keys_file, "%s", "}, ");

		fprintf(keys_file, "%c", '{');
		for (int b = 0; b < 6; ++b)
			fprintf(keys_file, "0x%02X,", comestero_keys_B[a][b]);
		fprintf(keys_file, "%s", "}, ");

	}

	fclose(keys_file);

}

void dump_parser(void) {
	 return;
}

void sas() {

	const uint8 keys1[32][6] = { {0xA0,0xA1,0xA2,0xA3,0xA4,0xA5}, {0x10,0x30,0x96,0x10,0x78,0x77}, {0x01,0xF9,0x8C,0xDE,0x95,0x68}, {0x11,0x82,0x18,0xFC,0xCC,0x4C}, {0x02,0x7C,0xE8,0x44,0xD0,0xA6}, {0x12,0x02,0x89,0x03,0x0B,0xC7}, {0x03,0x2E,0x38,0xC5,0x4A,0xEF}, {0x13,0x74,0x24,0xC6,0x48,0xA2}, {0x04,0xA1,0x9F,0xAE,0xB9,0x4D}, {0x14,0x33,0x9F,0x71,0xD3,0x19}, {0x05,0x3B,0x11,0x12,0x12,0x0F}, {0x15,0x8B,0xFC,0x62,0x0A,0xC5}, {0x06,0x40,0x06,0xD2,0xD4,0x8C}, {0x16,0x5B,0x71,0xD6,0x1E,0xA8}, {0x07,0xED,0x85,0x07,0x07,0x18}, {0x17,0x44,0x9E,0x1A,0x58,0x0A}, {0x08,0xFE,0xF4,0x4E,0x06,0xF7}, {0x18,0x65,0x05,0x6E,0x57,0x8A}, {0x09,0x1B,0x65,0x37,0x73,0x6D}, {0x19,0x83,0xA0,0xBA,0x88,0xF6}, {0x0A,0x53,0x49,0x24,0xBF,0xC0}, {0x1A,0x06,0x24,0xDA,0x3F,0x0E}, {0x0B,0x6D,0x71,0x3D,0x36,0xA6}, {0x1B,0x5C,0x86,0x0D,0x73,0x44}, {0x0C,0x15,0x0C,0x3D,0xB4,0x4E}, {0x1C,0x6D,0xE3,0x02,0x11,0x5C}, {0x0D,0x16,0x07,0x10,0x73,0xDF}, {0x1D,0x92,0xFA,0xEE,0x2E,0x12}, {0x0E,0xF4,0x5D,0x9A,0xBB,0xF1}, {0x1E,0x5D,0x86,0xF2,0x87,0xF2}, {0x0F,0xC6,0xDB,0x71,0x90,0x23}, {0x1F,0xF3,0xE8,0xC8,0x82,0x2B} };
	const uint8 keys2[32][6] = { {0xA0,0xA1,0xA2,0xA3,0xA4,0xA5}, {0x78,0x3E,0x57,0x2C,0x2A,0x09}, {0x69,0xF7,0x4D,0xE2,0xC7,0x16}, {0x79,0x8C,0xD9,0xC0,0x9E,0x32}, {0x6A,0x72,0x29,0x78,0x82,0xD8}, {0x7A,0x0C,0x48,0x3F,0x59,0xB9}, {0x6B,0x20,0xF9,0xF9,0x18,0x91}, {0x7B,0x7A,0xE5,0xFA,0x1A,0xDC}, {0x6C,0xAF,0x5E,0x92,0xEB,0x33}, {0x7C,0x3D,0x5E,0x4D,0x81,0x67}, {0x6D,0x35,0xD0,0x2E,0x40,0x71}, {0x7D,0x85,0x3D,0x5E,0x58,0xBB}, {0x6E,0x4E,0xC7,0xEE,0x86,0xF2}, {0x7E,0x55,0xB0,0xEA,0x4C,0xD6}, {0x6F,0xE3,0x44,0x3B,0x55,0x66}, {0x7F,0x4A,0x5F,0x26,0x0A,0x74}, {0x60,0xF0,0x35,0x72,0x54,0x89}, {0x70,0x6B,0xC4,0x52,0x05,0xF4}, {0x61,0x15,0xA4,0x0B,0x21,0x13}, {0x71,0x8D,0x61,0x86,0xDA,0x88}, {0x62,0x5D,0x88,0x18,0xED,0xBE}, {0x72,0x08,0xE5,0xE6,0x6D,0x70}, {0x63,0x63,0xB0,0x01,0x64,0xD8}, {0x73,0x52,0x47,0x31,0x21,0x3A}, {0x64,0x1B,0xCD,0x01,0xE6,0x30}, {0x74,0x63,0x22,0x3E,0x43,0x22}, {0x65,0x18,0xC6,0x2C,0x21,0xA1}, {0x75,0x9C,0x3B,0xD2,0x7C,0x6C}, {0x66,0xFA,0x9C,0xA6,0xE9,0x8F}, {0x76,0x53,0x47,0xCE,0xD5,0x8C}, {0x67,0xC8,0x1A,0x4D,0xC2,0x5D}, {0x77,0xFD,0x29,0xF4,0xD0,0x55} };

	const uint8 uid1[4] = {0x66, 0x88, 0xAA, 0x24};
	const uint8 uid2[4] = {0x33, 0x33, 0xA3, 0xFE};

	uint8 xor1[4];
	uint8 xor2[4];
	for(int i = 1; i < 32; ++i) 
	{

		for(int a = 0; a < 4; ++a) 
			xor1[a] = uid1[a] | keys1[i][a];

		for(int a = 0; a < 4; ++a) 
			xor2[a] = uid2[a] | keys2[i][a];


		int flag = 0;
		for (int a = 0; a < 4; ++a)
		{
			if (xor1[a] != xor2[a]) 
			{
				flag = 1;
			}

		
		}

		if (!flag) printf("si cazzo, %d - %02X\n", i, keys1[i][0]);

	}

}