# Assignment 6: Huffman Coding

## Description:

Reads file by the byte creating shorthand bit codes for each byte based on how frequently they show up. Then reconstructs file using shorthand version saving space.

## Build:

   $ make all
   This will build all exacutables.
   You can also build them separatly with $ make encode, and $ make decode.

## Running:

        ./encode -i[""] -o[""] -v -h

        ./decode -i[""] -o[""] -v -h

## Cleaning:

        $ make clean
        This will remove all the files genearted from $ make all.

## Command-Line Encode options:

| Argument | Description                                                                                   |
|----------|-----------------------------------------------------------------------------------------------|
| -v       | enables the verbose mode.                                   |
| -h       | displays program synopsis and usage.             |
| -i       | specifies the input file (default: stdin).                                             |
| -o       | specifies the output key file (default: stdout).                                                                                       

## Command-Line Decode options:

| Argument | Description                                                      |
|----------|------------------------------------------------------------------|
| -i       | specifies the input file to decode (default: stdin).            |
| -o       | specifies the output file to decode (default: stdout).          |
| -v       | enables verbose output.                                          |
| -h       | displays program synopsis and usage.                             |
