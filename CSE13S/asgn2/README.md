# Assignment 2: Numerical Integration

## Description:

	The exacutable integrate use Simpsom's Composite 1/3 rule to integrate a preprovided function between two required variables an n amount of times.

## Build:

	$ make integrate
	This will build all the .c files together to one exacutable file called integrate.

## Running:

	./integrate -[a-j] -p [low floating number] -q [high floating number] -n [integer number]

## Cleaning:

	$ make clean
	This will clear all the files generated from $ make integrate.

## Command-line options:
| Argument      | Description                                                                                                                                           |
|---------------|-------------------------------------------------------------------------------------------------------------------------------------------------------|
| -a            | Sets the function to integrate to Sqrt(1-x^4).                                                                                                        |
| -b            | Sets the function to integrate to 1/log(x).                                                                                                           |
| -c            | Sets the function to integrate to e^-x^2                                                                                                              |
| -d            | Sets the function to integrate to sin(x^2).                                                                                                           |
| -e            | Sets the function to integrate to cos(x^2).                                                                                                           |
| -f            | Sets the function to integrate to log(log(x)).                                                                                                        |
| -g            | Sets the function to integrate to sin(x)/x.                                                                                                           |
| -h            | Sets the function to integrate to (e^-x)/x.                                                                                                           |
| -i            | Sets the function to integrate to e^e^x.                                                                                                              |
| -j            | Sets the function to integrate to sqrt(sin^2(x)+cos^2(x)).                                                                                            |
| -n partitions | Sets the upper limit of partitions to use in the composite Simpson’s rule to partitions. This should have a default value of 100.                     |
| -p low        | Sets the low end of the interval to integrate over to low. This should not have a default value and must be specified each time the program is run.   |
| -q high       | Sets the high end of the interval to integrate over to high. This should not have a default value and must be specified each time the program is run. |
| -H            | Displays the program’s usage and synopsis.                                                                                                            |	
