CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -ggdb3 -gdwarf-4 
STYLE = -i -style=file
.PHONY: all clean format

all: httpserver

httpserver: httpserver.o
	$(CC) -g -o httpserver httpserver.o asgn2_helper_funcs.a

httpserver.o: httpserver.c 
	$(CC) $(CFLAGS) -c httpserver.c 

test: test.o
	$(CC) -g -o test test.o asgn2_helper_funcs.a

test.o: test.c
	$(CC) $(CFLAGS) -c test.c 

clean:
	rm -f httpserver.o httpserver test.o test foo.txt output.txt outputt.txt doodoo.txt error.txt outtemp.txt temp.txt val_log.txt diff.txt msgbody_diff.txt

format:
	$(CC)-format $(STYLE) httpserver.c

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./httpserver 8585

gdb:
	gdb ./test