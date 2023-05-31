CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -ggdb3 -gdwarf-4 
STYLE = -i -style=file
.PHONY: all clean format

all: queue.o

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -g -c queue.c 

test: test.o queue.o
	$(CC) -g -o test test.o queue.o

test.o: test.c queue.h
	$(CC) $(CFLAGS) -g -c test.c

clean:
	rm -f queue.o test test.o

format:
	$(CC)-format $(STYLE) queue.c queue.h

check: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose test