# Programming Assignment 8 Dictionary + Word Frequency: 

## Student Info:
    Name: Guillermo Alfaro
    CruzID: gualfaro
    ID #: 1801395
    Section: CSE101-1

## Description:
 Dictionary ADT. Has some uses! WordFrequency scans through entire file counting up how many times a word has appeared. All words are converted to lowercase.

## Build:

$ make 

This will build all files needed.


## Running:

        ./Order [infile] [outfile]
        ./WordFrequency [infile] [outfile]

## Cleaning:

        $ make clean
        This will remove all the files genearted from $ make.

## Memory Leak Check:
        
        $ make check
        This will before a valgrind leak test.

## Files:

Dictionary.cpp, Dictionary.h, DictionaryTest.cpp, Order.cpp, WordFrequency.cpp, Makefile, and README.md.
        

## Errors:
In ModelDictionaryTest I have 5 errors and leaks. I don't get these errors or leaks when running regular valgrind tests. So I think their edge case errors. Nonetheless, they are still errors. 