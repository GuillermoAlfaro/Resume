CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean format

all: hello

hello: hello.o
	$(CC) -g -o hello hello.o

hello.o: hello.c
	$(CC) $(CFLAGS) -c hello.c

clean:
	rm hello.o hello

format:
	$(CC)-format $(STYLE) hello.c