#------------------------------------------------------------------------------
#  Makefile for Graph ADT and FindPath
#
#  make FindPath    makes FindPath executable
#  make clean       removes object files
#  make check       runs valgrind to check for memory errors
#------------------------------------------------------------------------------

FindPath: FindPath.o Graph.o List.o
	gcc -std=c17 -Wall -g -o FindPath FindPath.o Graph.o List.o

FindPath.o: FindPath.c Graph.h List.h
	gcc -std=c17 -Wall -g -c FindPath.c

GraphTest: GraphTest.o Graph.o List.o
	gcc -std=c17 -Wall -g -o GraphTest GraphTest.o Graph.o List.o

GraphTest.o: GraphTest.c Graph.h List.h
	gcc -std=c17 -Wall -g -c GraphTest.c

Graph.o: Graph.c Graph.h List.h
	gcc -std=c17 -Wall -g -c Graph.c

List.o: List.c List.h
	gcc -std=c17 -Wall -g -c List.c

clean :
	rm -f FindPath FindPath.o Graph.o GraphTest GraphTest.o List.o

check : FindPath
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose FindPath infile outfile

TestCheck: GraphTest
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose GraphTest
