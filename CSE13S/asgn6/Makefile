CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean format


all: encode decode

encode: encode.o code.o huffman.o io.o node.o pq.o stack.o
	$(CC) -o encode encode.o code.o huffman.o io.o node.o pq.o stack.o -g

decode: decode.o code.o huffman.o io.o node.o pq.o stack.o
	$(CC) -o decode decode.o code.o huffman.o io.o node.o pq.o stack.o -g

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

clean:
	rm -f test test.o code.o huffman.o io.o node.o pq.o stack.o encode encode.o decode.o decode

format:
	$(CC)-format $(STYLE) code.c huffman.c io.c node.c pq.c stack.c huffman.h io.h node.h pq.h stack.h encode.c defines.h decode.c code.h

