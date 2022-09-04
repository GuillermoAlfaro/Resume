#include "List.h"
#include "Matrix.h"

void makeMatrix(){
    Matrix M = newMatrix(7);
    changeEntry(M, 1, 1, 3);
    changeEntry(M, 5, 1, 4);
    changeEntry(M, 2, 1, 5);
    changeEntry(M, 4, 1, 6);
    changeEntry(M, 3, 1, 22);
    changeEntry(M, 3, 6, 22);
    changeEntry(M, 3, 5, 78);
    changeEntry(M, 3, 4, 11);
    changeEntry(M, 7, 7, 7);
    puts("Matrix M");
    printMatrix(stdout, M);

    Matrix X = sum(M, M);
    puts("Matrix M+M");
    printMatrix(stdout, X);

}

void simpleMatrix(){
    Matrix M = newMatrix(3);
    Matrix D = newMatrix(3);
    changeEntry(M, 1, 1, 1);
    changeEntry(M, 2, 2, 2);
    changeEntry(M, 3, 1, 3);
    changeEntry(M, 1, 3, 4);
    changeEntry(M, 2, 1, 5);

    changeEntry(D, 2, 2, 10);
    changeEntry(D, 3, 3, 9);
    changeEntry(D, 2, 1, 8);
    changeEntry(D, 1, 2, 5);
    changeEntry(D, 3, 3, 2);
    changeEntry(D, 2, 3, 45);
    changeEntry(D, 1, 1, 1);
    puts("Matrix M");
    printMatrix(stdout, M);
    puts("\nMatrix D");
    printMatrix(stdout, D);
    Matrix MD = sum(M, D);
    puts("\nMatrix MD");
    printMatrix(stdout, MD);
}

void copyTest(){
    Matrix M = newMatrix(3);
    changeEntry(M, 1, 1, 1);
    changeEntry(M, 2, 2, 2);
    changeEntry(M, 3, 1, 3);
    changeEntry(M, 1, 3, 4);
    changeEntry(M, 2, 1, 5);
    puts("original");
    printMatrix(stdout, M);
    Matrix X = copy(M);
    puts("\ncopy");
    printMatrix(stdout, X);

    Matrix MX = sum(M, X);
    puts("\nsum");
    printMatrix(stdout, MX);

}

void scaldTest(){
    Matrix M = newMatrix(3);
    changeEntry(M, 1, 1, 1);
    changeEntry(M, 2, 2, 2);
    changeEntry(M, 3, 1, 3);
    changeEntry(M, 1, 3, 4);
    changeEntry(M, 2, 1, 5);
    puts("original");
    printMatrix(stdout, M);

    Matrix scald = scalarMult(4, M);
    puts("\nOG x 4");
    printMatrix(stdout, scald);

}

void diffTest(){
    Matrix M = newMatrix(5);
    changeEntry(M, 1, 1, 1);
    //changeEntry(M, 2, 2, 2);
    //changeEntry(M, 3, 1, 3);
    //changeEntry(M, 1, 3, 4);
    changeEntry(M, 2, 1, 5);
    Matrix D = scalarMult(3, M);

    changeEntry(M, 1, 2, 10);
    changeEntry(M, 3, 1, 100);
    puts("M Matrix");
    printMatrix(stdout, M);


    changeEntry(D, 2, 3, 20);


    puts("\nD Matrix");
    changeEntry(D, 5, 5, 100000);
    printMatrix(stdout, D);

    Matrix DM = diff(D, M);
    puts("\nD - M Matrix");
    printMatrix(stdout, DM);

    makeZero(DM);
    changeEntry(DM, 2, 3, 20);
    puts("\nClearing D-M and changing one entry");
    printMatrix(stdout, DM);



}

void freeTest(){
    Matrix M = newMatrix(5);
    changeEntry(M, 5, 5, 55);
    changeEntry(M, 5, 4, 20);
    changeEntry(M, 5, 3, 10);
    changeEntry(M, 2, 3, 44);
    changeEntry(M, 1, 1, 45);
    changeEntry(M, 2, 1, 47);
    Matrix S = newMatrix(5);
    changeEntry(S, 5, 5, 55);
    changeEntry(S, 5, 4, 20);
    changeEntry(S, 5, 3, 10);
    changeEntry(S, 2, 3, 44);
    changeEntry(S, 1, 1, 45);
    changeEntry(S, 2, 1, 47);
    puts("Matrix M");
    printMatrix(stdout, M);
    puts("\nMatrix S");
    printMatrix(stdout, S);
    printf("%d\n", equals(S, M));
    freeMatrix(&S);
    freeMatrix(&M);
}

void nonZeroTest(){
    Matrix M = newMatrix(5);
    changeEntry(M, 1, 1, 10);
    changeEntry(M, 5, 5, 100);
    changeEntry(M, 5, 1, 23);
    changeEntry(M, 2, 3, 1221);
    printMatrix(stdout, M);
    printf("\nNNZ total = %d\n", NNZ(M));
    freeMatrix(&M);

}

void timesTest(){
    Matrix M = newMatrix(7);
    changeEntry(M, 3, 2, 10);
    changeEntry(M, 7, 7, 77);
    changeEntry(M, 1, 1, 11);
    changeEntry(M, 4, 7, 22);
    Matrix copyM = copy(M);
    changeEntry(copyM, 2, 1, 50);
    changeEntry(copyM, 4, 1, 20);
    changeEntry(copyM, 3, 1, 111);

    changeEntry(M, 5, 5, 55);
    changeEntry(M, 5, 1, 1222);
    changeEntry(M, 5, 2, 99);
    changeEntry(M, 1, 3, 9000);
    changeEntry(M, 1, 5, 5);
    changeEntry(copyM, 1, 5, 5);
    puts("Matrix M");
    printMatrix(stdout, M);
    puts("\nMatrix copyM");
    printMatrix(stdout, copyM);

    Matrix MM = product(M, copyM);
    puts("\nMatrix MM");
    printMatrix(stdout, MM);


}

void sumTest(){
    Matrix A = newMatrix(5);
    Matrix B = newMatrix(5);

    changeEntry(A, 1, 1, 10);
    changeEntry(A, 2, 2, 22);
    changeEntry(A, 3, 1, 102);
    changeEntry(A, 3, 3, 11);
    changeEntry(A, 5, 5, 55);
    changeEntry(A, 1, 2, 20);
    changeEntry(A, 1, 5, 2222);

    changeEntry(B, 1, 2, 100);
    changeEntry(B, 1, 4, 54);
    changeEntry(B, 1, 1, 1);
    changeEntry(B, 5, 5, 23);
    changeEntry(B, 4, 2, 420);
    changeEntry(B, 5, 1, 16);

    puts("Matrix A");
    printMatrix(stdout, A);
    puts("\nMatrix B");
    printMatrix(stdout, B);
    puts("\nSum A + B");
    Matrix AB = sum(A, B);
    printMatrix(stdout, AB);




}

void transposeTest(){
    Matrix A = newMatrix(5);

    changeEntry(A, 1, 1, 10);
    changeEntry(A, 2, 2, 22);
    changeEntry(A, 3, 1, 102);
    changeEntry(A, 3, 3, 11);

    Matrix G = transpose(A);
    printMatrix(stdout, G);
}

void productTest(){
    Matrix M = newMatrix(3);
    changeEntry(M, 1, 1, 1.5);
    changeEntry(M, 1, 2, 2.6);
    changeEntry(M, 1, 3, 3.7);
    changeEntry(M, 2, 1, 4.8);
    changeEntry(M, 2, 2, 5.9);
    //changeEntry(M, 2, 3, 6);
    changeEntry(M, 3, 1, 7.1);
    changeEntry(M, 3, 2, 8.4);
    changeEntry(M, 3, 3, 9.5);

    Matrix B = newMatrix(3);
    changeEntry(B, 1, 1, 10.6);
    //changeEntry(B, 1, 2, 11);
    changeEntry(B, 1, 3, 12.2);
    //changeEntry(B, 2, 1, 13);
    changeEntry(B, 2, 2, 14.5);
    //changeEntry(B, 2, 3, 15);
    changeEntry(B, 3, 1, 16.5);
    //changeEntry(B, 3, 2, 17);
    //changeEntry(B, 3, 3, 18);

    Matrix MB = product(M, B);

    printMatrix(stdout, MB);
}

int diffNNZ(){
    Matrix A = newMatrix(100);
    changeEntry(A, 1, 1, -4);
    changeEntry(A, 1, 2, -2);
    changeEntry(A, 1, 3, 0);
    changeEntry(A, 2, 5, 4);
    changeEntry(A, 2, 1, -2);
    changeEntry(A, 3, 1, 2);
    changeEntry(A, 2, 2, -2);
    changeEntry(A, 3, 3, 0);
    Matrix PC = diff(A, A);
    printf("NNZ(PC) = %d\nNNZ(A) = %d\n", NNZ(PC), NNZ(A));
    if (NNZ(PC) != 0 || NNZ(A) != 6){
        return 1;
    }
    return 0;
}

void addTest(){
    Matrix M = newMatrix(5);
    changeEntry(M, 1, 1, 1.987);
    changeEntry(M, 1, 2, 23.4336);
    changeEntry(M, 1, 3, 54.956);
    changeEntry(M, 1, 5, 32.74);

    Matrix x = product(M, M);
    printMatrix(stdout, x);


}

void testTEsting(){
    Matrix M = newMatrix(10);

    changeEntry(M, 4, 7, -2402.8);
    changeEntry(M, 6, 5, -3604.1);
    changeEntry(M, 3,1 , -3013.9);
    changeEntry(M, 5,7 , -9982.4);
    changeEntry(M, 7,9 , -644.5);
    changeEntry(M, 8,7 , 1820.5);
    changeEntry(M, 1,4 , 5830.3);
    changeEntry(M, 6,4 , 5568.6);
    changeEntry(M, 8,9 , 9063.2);
    changeEntry(M, 6,7 , 8095.4);
    changeEntry(M, 5,1 , -1114.8);
    changeEntry(M, 2,4 , -6315.9);
    changeEntry(M, 1,1 , 8596.3);
    changeEntry(M, 4,5 , -9229.6);
    changeEntry(M, 8,8 , -4961.3);
    puts("Matrix M");
    printMatrix(stdout, M);
    Matrix B = newMatrix(10);
    puts("\nMatrix B");
    changeEntry(B, 5,2 , 816.8);
    changeEntry(B, 10,4 , 9159.8);
    changeEntry(B, 2,10 , -872.5);
    changeEntry(B, 1,2 , 835.0);
    changeEntry(B, 3,8 , 9269.1);
    changeEntry(B, 3,5 , -5437.9);
    changeEntry(B,7, 1, -8517.1);
    changeEntry(B,7, 3, 2903.1);
    changeEntry(B,6, 3, 848.7);
    changeEntry(B,4, 8, 6786.0);
    changeEntry(B,8, 9, 3664.7);
    changeEntry(B,9, 10, -2108.5);
    printMatrix(stdout, B);
    puts("product was gotten");
    Matrix G = product(M, B);

    puts("\nproduct of MB");
    printMatrix(stdout, G);


    freeMatrix(&M);
}

void moreTest(){
    Matrix M = newMatrix(3);
    Matrix X = newMatrix(3);

    changeEntry(M, 1, 1, 1);
    changeEntry(M, 1, 2, 2);
    changeEntry(M, 1, 3, 3);
    changeEntry(M, 2, 1, 4);
    changeEntry(M, 2, 2, 5);
    changeEntry(M, 2, 3, 6);

    changeEntry(X, 1, 1, 10);
    changeEntry(X, 1, 2, 11);
    changeEntry(X, 2, 1, 20);
    changeEntry(X, 2, 2, 21);
    changeEntry(X, 3, 1, 30);
    changeEntry(X, 3, 2, 31);


    Matrix G = product(M, X);
    printMatrix(stdout, G);
}

void twoTest(){
    Matrix A = newMatrix(4);
    Matrix B = newMatrix(4);


    changeEntry(A, 1, 1, 8);
    changeEntry(A, 1, 4, 5);
    changeEntry(B, 1, 3, 9);
    changeEntry(B, 1, 4, 6);

    Matrix X = product(A, B);
    printMatrix(stdout, X);

}

void newDifTest(){
    Matrix M = newMatrix(6);
    changeEntry(M, 1, 1, 1);
    changeEntry(M, 1, 3, 2);
    changeEntry(M, 1, 5, 3);
    changeEntry(M, 1, 6, 4);
    changeEntry(M, 2, 1, 1);
    changeEntry(M, 2, 2, 2);
    changeEntry(M, 2, 3, 3);
    changeEntry(M, 2, 4, 4);
    changeEntry(M, 2, 5, 5);
    changeEntry(M, 2, 6, 6);
    changeEntry(M, 4, 1, 2);
    changeEntry(M, 6, 1, 3);


    Matrix B = newMatrix(6);
    changeEntry(B, 1, 1, 1);
    changeEntry(B, 1, 2, 2);
    changeEntry(B, 1, 4, 45);
    changeEntry(B, 1, 6, 8);
    changeEntry(B, 2, 1, 1);
    changeEntry(B, 2, 2, 2);
    changeEntry(B, 2, 3, 3);
    changeEntry(B, 2, 4, 4);
    changeEntry(B, 2, 5, 5);
    changeEntry(B, 2, 6, 6);
    changeEntry(B, 3, 1, 2);
    changeEntry(B, 5, 1, 1);
    changeEntry(B, 6, 1, 3);

    Matrix D = diff(M, B);
    printMatrix(stdout, D);

}

int sumTestT(){
    Matrix A = newMatrix(10);
    Matrix B = newMatrix(10);
    Matrix *pC, *pD;
    changeEntry(A, 1, 1, 4);
    changeEntry(A, 1, 2, 2);
    changeEntry(A, 1, 3, 0);
    changeEntry(A, 2, 1, 2);
    changeEntry(A, 3, 1, 0);
    changeEntry(A, 2, 2, 2);
    changeEntry(A, 3, 3, 0);
    *pC = sum(A, A);
    if (NNZ(*pC) != 4 || NNZ(A) != 4)
        return 1;
    changeEntry(B, 1, 1, -4);
    changeEntry(B, 1, 2, 0);
    changeEntry(B, 2, 1, 0);
    changeEntry(B, 2, 2, -2);
    changeEntry(B, 2, 4, 2);
    changeEntry(B, 3, 1, 0);
    changeEntry(B, 3, 2, 2);
    changeEntry(B, 7, 8, 5);
    *pD = sum(A, B);
    if (NNZ(*pD) != 5)
        return 2;
    return 0;
}


int main(){
    //makeMatrix();
    //simpleMatrix();
    //copyTest();
    //scaldTest();
    //diffTest();
    //freeTest();
    //nonZeroTest();
    //timesTest();
    //sumTest();
    //transposeTest();
    //productTest();
    //addTest();
    //testTEsting();
    //moreTest();
    //twoTest();
    //newDifTest();
    sumTestT();
    return 0;

}
