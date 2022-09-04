#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 4
#
# make
# make
# make
# make
# make
#------------------------------------------------------------------------------

Sparse : Sparse.o List.o Matrix.o
	gcc -std=c17 -Wall -g -o Sparse Sparse.o List.o Matrix.o

Sparse.o: Sparse.c List.h Matrix.h
	gcc -std=c17 -Wall -g -c Sparse.c

MatrixTest : MatrixTest.o List.o Matrix.o
	gcc -std=c17 -Wall -g -o MatrixTest MatrixTest.o List.o Matrix.o

MatrixTest.o : MatrixTest.c List.h Matrix.h
	gcc -std=c17 -Wall -g -c MatrixTest.c

MatrixClient : MatrixClient.o List.o Matrix.o
	gcc -std=c17 -Wall -g -o MatrixClient MatrixClient.o List.o Matrix.o

MatrixClient.o : MatrixClient.c List.h Matrix.h
	gcc -std=c17 -Wall -g -c MatrixClient.c -g

ListTest: ListTest.o List.o entry.o
	gcc -std=c17 -Wall -g -o ListTest ListTest.o List.o entry.o

ListTest.o : ListTest.c List.h
	gcc -std=c17 -Wall -g -c ListTest.c -g

List.o : List.c List.h
	gcc -std=c17 -Wall -g -c List.c -g

Matrix.o : Matrix.c Matrix.h
	gcc -std=c17 -Wall -g -c Matrix.c -g

entry.o : entry.c entry.h
	gcc -std=c17 -Wall -g -c entry.c -g

clean :
	rm -f Lex ListTest ListTest.o List.o Matrix.o MatrixTest.o MatrixTest Sparse.o Sparse MatrixClient MatrixClient.o

check :
	valgrind --leak-check=full ./Sparse input output

checkLex :
	valgrind --leak-check=full Sparse in3 blah3

