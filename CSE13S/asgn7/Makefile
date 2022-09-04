CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean format


all: identify

identify: identify.o node.o parser.o bv.o ht.o speck.o bf.o text.o pq.o
	$(CC) -g -o identify identify.o node.o parser.o bv.o ht.o speck.o bf.o text.o pq.o -lm

identify.o: identify.c
	$(CC) $(CFLAGS) -c identify.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

text.o: text.c
	$(CC) $(CFLAGS) -c text.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

clean:
	rm -f bv.o speck.o bf.o ht.o parser.o text.o node.o pq.o lwords identify.o identify

format:
	$(CC)-format $(STYLE) bv.c speck.c bf.c ht.c parser.c text.c node.c pq.c identify.c metric.h
