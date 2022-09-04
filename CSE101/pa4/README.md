# Programming Assignment 4: Matrixes

## Student Info:
    Name: Guillermo Alfaro
    CruzID: gualfaro
    ID #: 1801395
    Section: CSE101-1

## Description:

Reads in file of ints and doubles and adds them to their appropriate matrix. Then performs different manipulations on said matrixes. 

## Build:

$ make Sparse

This will build Sparse.


## Running:

        ./Sparse [infile] [outfile]

## Cleaning:

        $ make clean
        This will remove all the files genearted from $ make Sparse.

## Memory Leak Check:
        
        $ make check
        This will before a valgrind leak test.

## Files:
		List.c 	        	- List ADT created in PA1
		
		List.h 	        	- Header file for List.c

            Matrix.c                - Matrix ADT created for this PA

            Matrix.h                - Header file for Matrix.c

            Sparse.c               - Main file.

            MatrixTest.c           - File to test different Matrix functionality

            ListTest.c             - File to test different List functionality
		
		Makefile	        - File to quickly compile program
		
		README.md	        - File that describes usage of program included files

## Errors:
So my program keeps timing out on the very last matrix model test. Unfortunate :( I believe it is the fact that I have somewhere around 2500+ errors lol.
As I go in and free the memory leaks and slowly fix the errors my program starts misbehaving having unforeseen outputs.
Obviously prioritising the Matrix Model Test is way more important that the first 10 points
of the output test, but It doesn't seem to fix no matter how many errors I attempt to patch.

So with this all in mind, I have decided to call it quits here and ask that I recieve a single bonus point for
fighting the good fight in CSE101. The date is Monday, May 2nd 8:44AM, o7!

9:10AM So in the span of my last diary entry I somehow fixed my Matrix Model Test and got it to pass 
LETSSS GOOO :)... but still so many f*en leaks. whatever we take those o7.