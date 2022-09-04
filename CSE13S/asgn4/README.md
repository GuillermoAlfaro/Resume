# Assignment 4: The Game of Life

## Description:

	Simulates The Game of Life created by John Horton Conway.

## Build:

	$ make all
	This will build the life exacutable.

## Running:

	./life -s -t -n[#] -i[""] -o[""] 

## Cleaning:

	$ make clean
	This will clear all the files generated from $ make sorting.

## Command-line options:
| Argument      | Description                                                                                                                                           |
|---------------|-------------------------------------------------------------------------------------------------------------------------------------------------------|
| -s            | Runs without using ncurses to show each generation.                                                                                                   |
| -t            | Makes universe toroidal.                                                                                                                              |
| -n            | Sets amount of generation to simulate.                                                                                                                |
| -i            | Takes in a specific file as input. (If not selected then stdin is default)                                                                            |
| -o            | Sets specific file to output to. (If not selected then stdout is defualt)                                                                             |

