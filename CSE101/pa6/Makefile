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

all : ListTest Arithmetic BigIntegerTest

Arithmetic : List.o Arithmetic.o BigInteger.o
	$(LINK) Arithmetic Arithmetic.o List.o BigInteger.o

Arithmetic.o : Arithmetic.cpp List.h BigInteger.h
	$(COMPILE) Arithmetic.cpp

ListTest : ListTest.o List.o
	$(LINK) ListTest ListTest.o List.o

ListTest.o : List.h ListTest.cpp
	$(COMPILE) ListTest.cpp

ListClient : List.h ListClient.o List.o
	$(LINK) ListClient ListClient.o List.o

ListClient.o : List.h ListClient.cpp
	$(COMPILE) ListClient.cpp

List.o : List.h List.cpp
	$(COMPILE) List.cpp

BigIntegerTest : List.h BigInteger.h BigIntegerTest.o List.o BigInteger.o
	$(LINK) BigIntegerTest BigIntegerTest.o List.o BigInteger.o

BigInteger : List.h BigInteger.h BigInteger.o List.o
	$(LINK) BigInteger BigInteger.o List.o -v

BigInteger.o : List.h BigInteger.h BigInteger.cpp
	$(COMPILE) BigInteger.cpp

BigIntegerTest.o : List.h BigInteger.h BigIntegerTest.cpp
	$(COMPILE) BigIntegerTest.cpp

clean :
	$(REMOVE) ListTest ListTest.o List.o ListClient.o ListClient BigInteger BigInteger.o BigIntegerTest BigIntegerTest.o Arithmetic.o Arithmetic

check : ListTest
	$(MEMCHECK) ListTest