#------------------------------------------------------------------------------
#  Makefile for Queue ADT
#
#  make             makes ListTest
#  make clean       removes object files
#  make check       runs valgrind to check for memory errors
#------------------------------------------------------------------------------

Lex: Lex.o List.o
	gcc -std=c17 -Wall -g -o Lex Lex.o List.o

Lex.o: Lex.c List.h
	gcc -std=c17 -Wall -g -c Lex.c

ListTest: ListTest.o List.o
	gcc -std=c17 -Wall -g -o ListTest ListTest.o List.o

ListTest.o: ListTest.c List.h
	gcc -std=c17 -Wall -g -c ListTest.c

List.o: List.c List.h
	gcc -std=c17 -Wall -g -c List.c

clean :
	rm -f ListTest ListTest.o List.o Lex Lex.o

check : Lex
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose Lex