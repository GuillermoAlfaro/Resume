//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA3
//************************************************
#include <stdlib.h>
#include <stdio.h>

#include "Matrix.h"

#define MAX_LEN 300

int main(int argc, char * argv[]){
    FILE *in, *out;
    char line[MAX_LEN];

    // check command line for correct number of arguments
    if( argc != 3 ){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    if( in==NULL ){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }


    int n, aa, bb;
    fgets(line, MAX_LEN, in);
    sscanf(line, "%d %d %d", &n, &aa, &bb);
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    fgets(line, MAX_LEN, in);
    int col, row;
    double value;
    for (int i = 0; i < aa; ++i) {
        fscanf(in, "%d %d %lf", &col, &row, &value);
        changeEntry(A, col, row, value);
    }
    for (int i = 0; i < bb; ++i) {
        fscanf(in, "%d %d %lf", &col, &row, &value);
        changeEntry(B, col, row, value);
    }

    fprintf(out, "A has %d non-zero entries:\n", aa);
    printMatrix(out, A);
    fprintf(out, "\nB has %d non-zero entries:\n", bb);
    printMatrix(out, B);
    fprintf(out, "\n(1.5)*A =\n");
    Matrix X = scalarMult(1.5, A);
    printMatrix(out, X);
    fprintf(out, "\nA+B =\n");
    Matrix SUM = sum(A, B);
    printMatrix(out, SUM);
    fprintf(out, "\nA+A =\n");
    Matrix aCopy = copy(A);
    SUM = sum(A, aCopy);
    printMatrix(out, SUM);
    fprintf(out, "\nB-A =\n");
    Matrix DIF = diff(B, A);
    printMatrix(out, DIF);
    fprintf(out, "\nA-A =\n");
    DIF = diff(A, aCopy);
    printMatrix(out, DIF);
    fprintf(out, "\nTranspose(A) =\n");
    Matrix tra = transpose(A);
    printMatrix(out, tra);
    fprintf(out, "\nA*B =\n");
    Matrix AB = product(A, B);
    printMatrix(out, AB);
    fprintf(out, "\nB*B = \n");
    Matrix bCopy = copy(B);
    Matrix BB = product(B, bCopy);
    printMatrix(out, BB);

    freeMatrix(&aCopy);
    freeMatrix(&DIF);
    freeMatrix(&tra);
    freeMatrix(&AB);
    freeMatrix(&bCopy);
    freeMatrix(&BB);
    freeMatrix(&SUM);
    freeMatrix(&X);
    freeMatrix(&A);
    freeMatrix(&B);
    fclose(out);
    fclose(in);
    return(0);
}
