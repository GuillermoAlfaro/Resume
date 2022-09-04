CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean format



all: sorting

sorting: sorting.o batcher.o heap.o insert.o quick.o stats.o
	$(CC) -o sorting sorting.o batcher.o insert.o quick.o heap.o stats.o -lm

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c batcher.c

sorting.o: sorting.c
	$(CC)  $(CFLAGS) -c sorting.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

insert.o: insert.c
	$(CC) $(CFLAGS) -c insert.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c

clean:
	rm -f sorting batcher.o sorting.o heap.o insert.o quick.o stats.o

format:
	$(CC)-format $(STYLE) stats.h sorting.c batcher.c heap.c insert.c quick.c stats.c

