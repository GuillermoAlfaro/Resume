CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -ggdb3
STYLE = -i -style=file
.PHONY: all clean format

all: memory

memory: memory.o
	$(CC) -g -o memory memory.o

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c

test: test.o
	$(CC) -g -o test test.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

gutter: gutter.o
	$(CC) -g -o gutter gutter.o

gutter.o: gutter.c
	$(CC) $(CFLAGS) -c gutter.c

clean:
	rm -f memory.o memory test.o test gutter gutter.o

format:
	$(CC)-format $(STYLE) memory.c test.c gutter.c