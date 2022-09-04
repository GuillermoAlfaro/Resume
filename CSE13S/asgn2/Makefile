CC = clang
CLANG = -Wall -Wextra -Werror -Wpedantic
STYLE = -i -style=file
.PHONY: all clean

all: integrate

integrate: integrate.o functions.o mathlib.o
	$(CC) -o integrate integrate.o functions.o mathlib.o

integrate.o: integrate.c
	$(CC) $(CFLAGS) -c integrate.c

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c

mathlib.o: mathlib.c
	$(CC) $(CFLAGS) -c mathlib.c

clean:
	rm -f integrate integrate.o functions.o mathlib.o

format:
	$(CC)-format $(STYLE) integrate.c functions.c mathlib.c functions.h mathlib.h
