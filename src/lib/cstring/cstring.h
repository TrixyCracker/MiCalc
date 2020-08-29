#pragma once

typedef unsigned char uint8;

int string_to_int(const char * _Str);
uint8 char_to_int(const char _Char);

int string_to_uint8_array(uint8 * _Dest, const char * _Str);
uint8 char_to_uint8(const char _Char);

void memory_copy(void * _Dest, const void * _Src, const int _Size);

int string_lenght(const char * _Str);

void string_set(char * _Str, const int _Size, const int _Value);

void string_copy(char * _Dest, const char * _Str);

int string_compare(const char * _Str1, const char * _Str2);

void string_addstring(char * _Dest, const char * _Str);
void string_addchar(char * _Str, const char _c);

void string_lowercase(char * _Dest, const char * _Str);
char char_lowercase(const char _Char);

void string_uppercase(char * _Dest, const char * _Str);
char char_uppercase(const char _Char);