#pragma once

typedef unsigned char uint8;

int string_to_uint8(uint8 * _Dest, char * _Str);
uint8 char_to_uint8(char _c);

void memory_copy(void * _Dest, const void * _Src, int _Size);

int string_lenght(const char * _Str);

void string_set(char * _Str, const int _Size, const int _Value);

void string_copy(char * _Dest, const char * _Str);

int string_compare(const char * _Str1, const char * _Str2);

void string_addstring(char * _Dest, const char * _Str);
void string_addchar(char * _Str, const char _c);

void string_lowercase(char * _Dest, const char * _Str);
void string_uppercase(char * _Dest, const char * _Str);

char char_lowercase(const char _char);
char char_uppercase(const char _char);