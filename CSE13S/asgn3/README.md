# Assignment 3: Sorting

## Description:

	Test four different types of sorting methods, Insertion sort, Batcher sort, Heap sort, and Quick sort. Each can me tested all at once or individually, outputs can either be all random number generated or first -p[]. 

## Build:

	$ make sorting
	This will build the sorting exacutable.

## Running:

	./sorting -a -i -h -q -b -n[#] -p[#] -H

## Cleaning:

	$ make clean
	This will clear all the files generated from $ make sorting.

## Command-line options:
| Argument      | Description                                                                                                                                           |
|---------------|-------------------------------------------------------------------------------------------------------------------------------------------------------|
| -a            | Runs all sorting algorithms.                                                                                                                          |
| -i            | Runs insertion sort.                                                                                                                                  |
| -h            | Runs heap sort.                                                                                                                                       |
| -q            | Runs quick sort.                                                                                                                                      |
| -b            | Runs batcher sort.                                                                                                                                    |
| -n            | Sets amount of numbers to be randomly generated to array, default is 100.                                                                             |
| -p            | Sets amount of sorted array to print on screen, default is 100.                                                                                       |
| -H            | Displays help screen.                                                                                                                                 |
| -r            | Sets specific seed, default is 1337143.                                                                                                               |

