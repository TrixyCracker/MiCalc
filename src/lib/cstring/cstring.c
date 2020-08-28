#include "cstring.h"

void memory_copy(void * _Dest, const void * _Src, int _Size) {
    char * dest = _Dest;
    const char * src = _Src;

    for (int i = 0; i < _Size; i++) {

        *(dest + i) = *(src + i);

    }

}

int string_lenght(const char * _Str) {
    int lenght = 0;

    while(_Str[lenght] != '\0') {
        lenght += 1;
    }

    return lenght;
}

void string_set(char * _Str, const int _Size, const int _Value) {
    for (int i = 0; i < _Size; i++) {
        _Str[i] = _Value;
    }
}

void string_copy(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++) {

        _Dest[i] = _Str[i];

    }

    _Dest[strlen] = '\0';
}

int string_compare(const char * _Str1, const char * _Str2) {
    int flag = 0;
    int strlen = string_lenght(_Str1);

    for (int i = 0; i <= strlen; i++) {

        if (_Str1[i] != _Str2[i]) {
            flag = 1;

            break;
        }

    }

    if (flag) 
        return 0;
    else 
        return 1;
}

void string_addstring(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i <= strlen; i++) {

        string_addchar(_Dest, _Str[i]);

    }

}

void string_addchar(char * _Dest, const char _c) {

    int strlen = string_lenght(_Dest);

    _Dest[strlen] = _c;
    _Dest[strlen + 1] = '\0';

}

void string_lowercase(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++) {

        _Dest[i] = char_lowercase(_Str[i]);

    }

    _Dest[strlen] = '\0';
}

void string_uppercase(char * _Dest, const char * _Str) {
    int strlen = string_lenght(_Str);

    for (int i = 0; i < strlen; i++) {

        _Dest[i] = char_uppercase(_Str[i]);

    }

    _Dest[strlen] = '\0';
}

char char_lowercase(const char _char) {
    char lower[27] = "abcdefghijklmnopqrstuvwxyz";
    char upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 27; i++) {

        if (_char == upper[i]) return lower[i];

    }

    return _char;
}

char char_uppercase(const char _char) {
    char lower[27] = "abcdefghijklmnopqrstuvwxyz";
    char upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 27; i++) {

        if (_char == lower[i]) return upper[i];

    }

    return _char;
}