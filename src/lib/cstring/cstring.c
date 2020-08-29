#include "cstring.h"

int string_to_uint8(uint8 * _Dest, char * _Str) 
{

    int lenght = string_lenght(_Str);

    for (int i = 0; i < lenght; ++i) {

        if (char_to_uint8(_Str[i]) == 0xFF)
            return 0;

    }

    for (int i = 0; i < lenght / 2; ++i)
        _Dest[i] =  char_to_uint8(_Str[i * 2]) * 16 + char_to_uint8(_Str[i * 2 + 1]);

    return 1;
}

uint8 char_to_uint8(char _c)
{

	if (_c == '0')
		return 0x0;
	else if (_c == '1')
		return 0x1;
	else if (_c == '2')
		return 0x2;
	else if (_c == '3')
		return 0x3;
	else if (_c == '4')
		return 0x4;
	else if (_c == '5')
		return 0x5;
	else if (_c == '6')
		return 0x6;
	else if (_c == '7')
		return 0x7;
	else if (_c == '8')
		return 0x8;
	else if (_c == '9')
		return 0x9;
	else if (_c == 'A' || _c == 'a')
		return 0xA;
	else if (_c == 'B' || _c == 'b')
		return 0xB;
	else if (_c == 'C' || _c == 'c')
		return 0xC;
	else if (_c == 'D' || _c == 'd')
		return 0xD;
	else if (_c == 'E' || _c == 'e')
		return 0xE;
	else if (_c == 'F' || _c == 'f')
		return 0xF;
	else
		return 0xFF;

}

void memory_copy(void * _Dest, const void * _Src, int _Size) 
{
    char * dest = _Dest;
    const char * src = _Src;

    for (int i = 0; i < _Size; i++)
        *(dest + i) = *(src + i);

}

int string_lenght(const char * _Str) 
{
    int lenght = 0;

    while(_Str[lenght] != '\0')
        lenght += 1;

    return lenght;
}

void string_set(char * _Str, const int _Size, const int _Value) 
{

    for (int i = 0; i < _Size; i++) 
        _Str[i] = _Value;

}

void string_copy(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++)
        _Dest[i] = _Str[i];

    _Dest[strlen] = '\0';
}

int string_compare(const char * _Str1, const char * _Str2) {
    int strlen = string_lenght(_Str1);

    for (int i = 0; i <= strlen; i++) {

        if (_Str1[i] != _Str2[i])
            return 0;

    }

    return 1;
}

void string_addstring(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i <= strlen; i++)
        string_addchar(_Dest, _Str[i]);

}

void string_addchar(char * _Dest, const char _c) {

    int strlen = string_lenght(_Dest);

    _Dest[strlen] = _c;
    _Dest[strlen + 1] = '\0';

}

void string_lowercase(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++)
        _Dest[i] = char_lowercase(_Str[i]);

    _Dest[strlen] = '\0';
}

void string_uppercase(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++)
        _Dest[i] = char_uppercase(_Str[i]);

    _Dest[strlen] = '\0';
}

char char_lowercase(const char _char) {
    char lower[27] = "abcdefghijklmnopqrstuvwxyz";
    char upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 27; i++) {

        if (_char == upper[i]) 
            return lower[i];

    }

    return _char;
}

char char_uppercase(const char _char) {
    char lower[27] = "abcdefghijklmnopqrstuvwxyz";
    char upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 27; i++) {

        if (_char == lower[i]) 
            return upper[i];

    }

    return _char;
}