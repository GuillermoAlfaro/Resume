# Programming Assignment 2: Breadth First Search and Shortest Paths

## Student Info:
    Name: Guillermo Alfaro
    CruzID: gualfaro
    ID #: 1801395
    Section: CSE101-1

## Description:

Reads in file of vertex integers and performs a breadth first search and finds the shortest path between two nodes if it exists.

## Build:

$ make FindPath

This will build FindPath.


## Running:

        ./FindPath [infile] [outfile]

## Cleaning:

        $ make clean
        This will remove all the files genearted from $ make FindPath.

## Memory Leak Check:
        
        $ make check
        This will before a valgrind leak test.

## Files:
		List.c 		- List ADT created in PA1
		
		List.h 		- Header file for List.c
		
		Graph.c 	- Graph ADT that has that can you perform BFS on to find
					  distance between vertexs and most optimal path between them
					  if it exists.
					  
		Graph.h		- Header file for Graph.c
		
		GraphTest.c	- File to test different scenarios and functionality in
					  Graph.c
					  
		FindPath.c	- Main file that acts as interface between Graph and user
		
		Makefile	- File to quickly compile program
		
		README.md	- File that describes usage of program included files