CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic -g -v $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)
STYLE = -i -style=file
.PHONY: all clean format

all: encrypt decrypt keygen

encrypt: encrypt.o 
	$(CC) -o encrypt encrypt.o randstate.o numtheory.o rsa.o $(LFLAGS)

decrypt: decrypt.o randstate.o numtheory.o rsa.o
	$(CC) -o decrypt decrypt.o randstate.o numtheory.o rsa.o $(LFLAGS)

keygen: keygen.o randstate.o numtheory.o rsa.o
	$(CC) -g -o keygen keygen.o randstate.o numtheory.o rsa.o $(LFLAGS)

test: randstate.o test.o numtheory.o rsa.o
	$(CC) -o test randstate.o test.o numtheory.o rsa.o $(LFLAGS)

numtheory.o: numtheory.c
	$(CC) -c numtheory.c

randstate.o: randstate.c
	$(CC) -c randstate.c -g

test.o: test.c
	$(CC) -c test.c -g

rsa.o: rsa.c
	$(CC) -c rsa.c -g

encrypt.o: encrypt.c randstate.o numtheory.o rsa.o
	$(CC) $(CFLAGS) -c encrypt.c randstate.c numtheory.c rsa.c -g

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c -g

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c -g

clean:
	rm -f encrypt decrypt keygen encrypt.o decrypt.o keygen.o numtheory.o rsa.o randstate.o

format:
	$(CC)-format $(STYLE) encrypt.c decrypt.c keygen.c randstate.c randstate.h rsa.c rsa.h numtheory.c numtheory.o

