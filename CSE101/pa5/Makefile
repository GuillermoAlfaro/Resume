#------------------------------------------------------------------------------
#  Makefile for List ADT
#
#  make             makes ListTest
#  make clean       removes object files
#  make check       runs valgrind to check for memory errors
#------------------------------------------------------------------------------

COMPILE          = g++ -g -std=c++17 -Wall -c
LINK             = g++ -g -std=c++17 -Wall -o
REMOVE           = rm -f
MEMCHECK         = valgrind --leak-check=full

all : ListTest Shuffle

ListTest : ListTest.o List.o
	$(LINK) ListTest ListTest.o List.o

Shuffle : Shuffle.o List.o
	$(LINK) Shuffle Shuffle.o List.o

Shuffle.o : List.h Shuffle.cpp
	$(COMPILE) Shuffle.cpp

ListTest.o : List.h ListTest.cpp
	$(COMPILE) ListTest.cpp

ListClient : List.h ListClient.o List.o
	$(LINK) ListClient ListClient.o List.o

ListClient.o : List.h ListClient.cpp
	$(COMPILE) ListClient.cpp

List.o : List.h List.cpp
	$(COMPILE) List.cpp

ModelListTest.o : List.h
	$(COMPILE) ModelListTest.cpp

ModelListTest : ModelListTest.o List.h
	$(LINK) ModelListTest ModelListTest.o List.o

clean :
	$(REMOVE) ListTest ListTest.o List.o Shuffle.o ListClient.o Shuffle ListClient

check : ListTest
	$(MEMCHECK) ListTest