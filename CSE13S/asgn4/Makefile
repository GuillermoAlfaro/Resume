CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean format



all: life

life: life.o universe.o
	$(CC) -o life life.o universe.o -lncurses

life.o: life.c
	$(CC) $(CFLAGS) -c life.c

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

clean:
	rm -f life life.o universe.o

format:
	$(CC)-format $(STYLE) universe.h universe.c life.c

