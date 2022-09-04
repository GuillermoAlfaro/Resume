#------------------------------------------------------------------------------
#  Makefile for Graph ADT and FindPath
#
#  make FindPath    makes FindPath executable
#  make clean       removes object files
#  make check       runs valgrind to check for memory errors
#------------------------------------------------------------------------------

FindComponents: FindComponents.o Graph.o List.o
	gcc -std=c17 -Wall -g -o FindComponents FindComponents.o Graph.o List.o

FindComponents.o: FindComponents.c Graph.h List.h
	gcc -std=c17 -Wall -g -c FindComponents.c

GraphTest: GraphTest.o Graph.o List.o
	gcc -std=c17 -Wall -g -o GraphTest GraphTest.o Graph.o List.o

GraphTest.o: GraphTest.c Graph.h List.h
	gcc -std=c17 -Wall -g -c GraphTest.c

Graph.o: Graph.c Graph.h List.h
	gcc -std=c17 -Wall -g -c Graph.c

List.o: List.c List.h
	gcc -std=c17 -Wall -g -c List.c

clean :
	rm -f Graph.o GraphTest GraphTest.o List.o Graph.o FindComponents.o FindComponents

check : FindComponents
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./FindComponents input output

TestCheck: GraphTest
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose GraphTest
