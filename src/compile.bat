@echo off
gcc -c lib\cstring\cstring.c -o build\cstring.o
ar rcs build\libcstring.a build\cstring.o
gcc main.c -Lbuild -lcstring
pause