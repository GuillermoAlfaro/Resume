#------------------------------------------------------------------------------
#  Makefile for Dictionary ADT
#
#  make             makes DictionaryTest
#  make clean       removes object files
#  make check       runs valgrind to check for memory errors
#------------------------------------------------------------------------------

COMPILE          = g++ -g -std=c++17 -Wall -c
LINK             = g++ -g -std=c++17 -Wall -o
REMOVE           = rm -f
MEMCHECK         = valgrind --leak-check=full --num-callers=12 --track-origins=yes

all : DictionaryTest Order WordFrequency

WordFrequency : WordFrequency.o Dictionary.o
	$(LINK) WordFrequency WordFrequency.o Dictionary.o

WordFrequency.o : Dictionary.h WordFrequency.cpp
	$(COMPILE) WordFrequency.cpp

DictionaryTest : DictionaryTest.o Dictionary.o Dictionary.h
	$(LINK) DictionaryTest DictionaryTest.o Dictionary.o

DictionaryTest.o : Dictionary.h DictionaryTest.cpp
	$(COMPILE) DictionaryTest.cpp

Dictionary.o : Dictionary.h Dictionary.cpp
	$(COMPILE) Dictionary.cpp

Order : Order.o Dictionary.o Dictionary.h
	$(LINK) Order Order.o Dictionary.o

Order.o : Order.cpp Dictionary.h
	$(COMPILE) Order.cpp

clean :
	$(REMOVE) DictionaryTest DictionaryTest.o Dictionary.o Order.o Order WordFrequency WordFrequency.o

check : Order
	$(MEMCHECK) ./Order infile5.txt out