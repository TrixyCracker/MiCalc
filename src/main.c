//Libraries
#include <stdio.h>

//Typedef
typedef unsigned char u8;

//Declaration of functions 
void calculate_keyA(u8 _Dest[6], u8 _UID[4], int _Block);
void calculate_keyB(u8 _Dest[6], u8 _UID[4], int _Block);

void uid_to_u8(u8 _Dest[8], char _UID[9]);
u8 char_to_u8(char _c);

//Functions
int main(void) {

	u8 KeyA[5][6] = {
		[0] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 }
	};
	u8 KeyB[5][6] = {
		[0] = { 0xB4, 0xC1, 0x32, 0x43, 0x9E, 0xEF }
	};

	char UID_string[9];
	printf("Enter the UID of MiZip: 0x");
	scanf("%s", UID_string);

	//Check if is valid UID
	for (int i = 0; i < 8; i++) 
	{	
		if ( char_to_u8(UID_string[i]) == 0xFF ) 
		{
			printf("[!] Please enter a valid UID. \n");
			return 1;
		}
	}
	//---

	//UID string to uint8 array
	u8 UID[4];
	for (int i = 0; i < 4; i++) {
		UID[i] = (char_to_u8(UID_string[i * 2]) << 4) + char_to_u8(UID_string[(i * 2) + 1]);
	}
	//---

	printf("UID: 0x");
	for (int i = 0; i < 4; i++) printf("%02X", UID[i]);
	printf("\n");

	for (int i = 0; i < 5; i++)
	{

		calculate_keyA(KeyA[i], UID, i);
		calculate_keyB(KeyB[i], UID, i);

		printf("\tBlock %d: \n", i);

		printf("\t\tKey A: 0x");
		for (int i2 = 0; i2 < 6; i2++) printf("%02X", KeyA[i][i2]);
		printf("\n");

		printf("\t\tKey B: 0x");
		for (int i2 = 0; i2 < 6; i2++) printf("%02X", KeyB[i][i2]);
		printf("\n");

	}
	
	fflush(stdin);
	getchar();

	return 0;
}

void calculate_keyA(u8 _Dest[6], u8 _UID[4], int _Block) 
{
	//Costants
	const int byte_rules[6] = { 0, 1, 2, 3, 0, 1 };

	const u8 xor_table_keyA[5][6] = {
		[1] = { 0x09, 0x12, 0x5A, 0x25, 0x89, 0xE5 },
		[2] = { 0xAB, 0x75, 0xC9, 0x37, 0x92, 0x2F },
		[3] = { 0xE2, 0x72, 0x41, 0xAF, 0x2C, 0x09 },
		[4] = { 0x31, 0x7A, 0xB7, 0x2F, 0x44, 0x90 }
	};

	//Code
	if (_Block == 0) return;

	for (int i = 0; i < 6; i++) 
	{
		_Dest[i] = _UID[byte_rules[i]] ^ xor_table_keyA[_Block][i];
	}

}

void calculate_keyB(u8 _Dest[6], u8 _UID[4], int _Block) 
{
	//Costants
	const int byte_rules[6] = { 2, 3, 0, 1, 2, 3 };

	const u8 xor_table_keyB[5][6] = {
		[1] = { 0xF1, 0x2C, 0x84, 0x53, 0xD8, 0x21 },
		[2] = { 0x73, 0xE7, 0x99, 0xFE, 0x32, 0x41 },
		[3] = { 0xAA, 0x4D, 0x13, 0x76, 0x56, 0xAE },
		[4] = { 0xB0, 0x13, 0x27, 0x27, 0x2D, 0xFD }
	};

	//Code
	if (_Block == 0) return;

	for (int i = 0; i < 6; i++) 
	{
		_Dest[i] = _UID[byte_rules[i]] ^ xor_table_keyB[_Block][i];
	}

}

u8 char_to_u8(char _c) 
{

	if (_c == '0') return 0x00;
	else if (_c == '1') return 0x1;
	else if (_c == '2') return 0x2;
	else if (_c == '3') return 0x3;
	else if (_c == '4') return 0x4;
	else if (_c == '5') return 0x5;
	else if (_c == '6') return 0x6;
	else if (_c == '7') return 0x7;
	else if (_c == '8') return 0x8;
	else if (_c == '9') return 0x9;
	else if (_c == 'A' || _c == 'a') return 0xA;
	else if (_c == 'B' || _c == 'b') return 0xB;
	else if (_c == 'C' || _c == 'c') return 0xC;
	else if (_c == 'D' || _c == 'd') return 0xD;
	else if (_c == 'E' || _c == 'e') return 0xE;
	else if (_c == 'F' || _c == 'f') return 0xF;
	else return 0xFF;

}