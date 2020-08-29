
all: micalc

micalc: main.o cstring.o
	gcc build/main.o build/cstring.o -o release/release_LNX

main.o: main.c 
	gcc -Wall -c main.c -o build/main.o

cstring.o: lib/cstring/cstring.c lib/cstring/cstring.h 
	gcc -Wall -c lib/cstring/cstring.c -o build/cstring.o

clean:
	rm build/* -rf