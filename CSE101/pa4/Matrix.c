#include "List.h"
#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structs --------------------------------------------------------------------------------
typedef struct EntryObj* Entry;
typedef struct EntryObj {
    int j;
    double value;
} EntryObj;

typedef struct Matrix {
    List *array;
    int size;
} MatrixObj;

Entry newEntry(int j, double value){
    Entry E = malloc(sizeof(EntryObj));
    E->j = j;
    E->value = value;
    return E;
}

void freeEntry(Entry* E){
    if (E != NULL && (*E) != NULL){
        free(*E);
        *E = NULL;
    }

}

void printRow(FILE* out, List L, int i);

void printEntry(FILE* out, Entry E);

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    Matrix M = (Matrix) malloc(sizeof(MatrixObj));
    M->array = (List *) malloc((n+1) * sizeof(List));
    for (int i = 1; i < n+1; i++) {
        M->array[i] = newList();
    }
    M->size = n;
    return M;
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    if (pM != NULL && *pM != NULL){
        for (int i = 1; i < (*pM)->size+1 ; ++i) {
            List L = (*pM)->array[i];
            moveFront(L);
            for (int j = 0; j < length(L); ++j) {
                if (index(L)+1 != length(L)) {
                    moveNext(L);
                }
            }
            freeList(&L);
            L = NULL;
        }
        free((*pM)->array);
        (*pM)->array = NULL;
        free(*pM);
        (*pM) = NULL;
    }
}

// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
    return M->size;
}

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    int counter = 0;
    for (int i = 1; i < size(M)+1; ++i) {
        counter += length(M->array[i]);
    }
    return counter;
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
    if (A->size != B->size){
        return false;
    }
    for (int i = 1; i < A->size+1; ++i) {
        List LL = A->array[i];
        List D = B->array[i];
        if (length(LL) != length(D)){
            return false;
        }
        if (length(LL) > 0) {
            moveFront(D);
            moveFront(LL);
            for (int j = 0; j < length(LL); ++j) {
                Entry G = get(LL);
                Entry X = get(D);
                if (get(LL) == (void*)-1 && get(D) == (void*)-1){
                    continue;
                }
                if (G->value != X->value){
                    return false;
                }
                if (G->j != X->j){
                    return false;
                }
                if (index(LL) + 1 < length(LL)) {
                    moveNext(LL);
                    moveNext(D);
                }
            }
        }
    }
    return true;
}

// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M) {
    List L;
    for (int i = 1; i < M->size + 1; ++i) {
        L = M->array[i];
        clear(L);
    }
    /*free(M->array);
    M->array = NULL;
    int n = M->size;
    M->array = (List *) malloc(n * sizeof(List));
    for (int i = 1; i < n + 1; i++) {
        M->array[i] = newList();
    }*/
}

// Inserts u into L sorted from least to greatest.
void sort_insert(List L, Entry E){
    if (E->value == 0){
        return;
    }
    if (length(L) == 0){
        append(L, E);
        return;
    }
    moveFront(L);
    for (int j = 0; j < length(L); ++j) {
        Entry G = get(L);
        if (G->j > E->j){
            insertBefore(L, E);
            return;
        } else if (get(L) == back(L)){
            insertAfter(L, E);
            return;
        } else {
            moveNext(L);
        }
    }
}

double addVerify(List L, int j){
    moveFront(L);
    for (int i = 0; i < length(L); ++i) {
        Entry E = get(L);
        if (E->j == j){
            double prevValue = E->value;
            delete(L);
            return prevValue;
        }
        if (length(L) == 1){
            break;
        }
        moveNext(L);
    }
    return 0;
}

void addEntry(Matrix M, int i, int j, double x){
    if (1 > i || i > size(M) || 1 > j || j > size(M)){
        return;
    } else {
        double prevValue = addVerify(M->array[i], j);
        Entry E = newEntry(j, x + prevValue);
        sort_insert( M->array[i], E);
    }
    return;
}

void subEntry(Matrix M, Matrix X, int i, int j, double x){
    if (1 > i || i > size(M) || 1 > j || j > size(M)){
        return;
    } else {
        double existingValue = addVerify(X->array[i], j);
        Entry E = newEntry(j, existingValue - x);
        sort_insert( M->array[i], E);
    }
    return;
}

void verify(List L, int j){
    moveFront(L);
    for (int i = 0; i < length(L); ++i) {
        Entry E = get(L);
        if (E->j == j){
            freeEntry(&E);
            delete(L);
            return;
        }
        if (length(L) == 1){
            break;
        }
        moveNext(L);
    }
    return;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    if (1 > i || i > size(M) || 1 > j || j > size(M)) {
        return;
    } else if (x == 0) {
        verify(M->array[i], j);
        return;
    } else {
        verify(M->array[i], j);
        sort_insert( M->array[i], newEntry(j, x));
    }
    return;

}

// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size+1; ++i) {
        List L = A->array[i];
        moveFront(L);
        for (int j = 0; j < length(L); ++j) {
            Entry E = get(L);
            changeEntry(M, i, E->j, E->value);
            if (length(L) == 1){
                break;
            }
            moveNext(L);
        }
    }
    return M;
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size+1; ++i) {
        List L = A->array[i];
        moveFront(L);
        for (int j = 0; j < length(L); ++j) {
            Entry E = get(L);
            changeEntry(M, E->j, i, E->value);
            if (length(L) == 1){
                break;
            }
            moveNext(L);
        }
    }
    return M;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size+1; ++i) {
        List L = A->array[i];
        moveFront(L);
        for (int j = 0; j < length(L); ++j) {
            Entry E = get(L);
            changeEntry(M, i, E->j, E->value * x);
            if (length(L) == 1){
                break;
            }
            moveNext(L);
        }
    }
    return M;
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    if (size(A) != size(B)){
        return A;
    }
    Matrix Bb;
    if (equals(A, B)){
        Bb = copy(B);
    } else {
        Bb = B;
    }
    int largerLength;
    Matrix M = newMatrix(size(A));
    bool aLever = false;
    bool bLever = false;
    for (int i = 1; i < size(A)+1; ++i) {
        List AA = A->array[i];
        List BB = Bb->array[i];
        if (length(AA) >= length(BB)){
            largerLength = length(AA);
        } else {
            largerLength = length(BB);
        }
        moveFront(AA);
        moveFront(BB);
        Entry AAA = get(AA);
        Entry BBB = get(BB);
        if (length(AA)>0){
            aLever = true;
        }
        if (length(BB)>0){
            bLever = true;
        }
        for (int j = 0; j < largerLength; ++j) {
            if (aLever){
                AAA = get(AA);
                addEntry(M, i, AAA->j, AAA->value);
            }
            if (bLever){
                BBB = get(BB);
                addEntry(M, i, BBB->j, BBB->value);
            }
            aLever = false;
            bLever = false;
            if (largerLength == 1){break;}
            if (index(AA)+1 != length(AA)){
                moveNext(AA);
                aLever = true;
            }
            if (index(BB)+1 != length(BB)){
                moveNext(BB);
                bLever = true;
            }
        }
    }

    return M;
}

void subbing(Matrix M, List L, bool swit, int in){
    moveFront(L);
    for (int i = 0; i < length(L); ++i) {
        Entry E = get(L);
        if (swit){
            changeEntry(M, in, E->j, 0 - E->value);
        } else {
            changeEntry(M, in, E->j, E->value);
        }
        if (index(L)+1 != length(L)){
            moveNext(L);
        }
    }
}

int moveOver(List A){
    if (index(A)+1 != length(A)){
        moveNext(A);
        return 0;
    } else {
        return -1;
    }
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    if (size(A) != size(B)){
        return A;
    }
    Matrix M = newMatrix(size(A));
    if (equals(A, B)){
        return M;
    }
    for (int i = 1; i < size(A)+1; ++i) {
        List aList = A->array[i];
        List bList = B->array[i];
        int aFin = 0;
        int bFin = 0;
        if (length(aList) == 0 && length(bList) > 0){
            subbing(M, bList, true, i);
            continue;
        }
        if (length(bList) == 0 && length(aList) > 0){
            subbing(M, aList, false, i);
            continue;
        }
        moveFront(aList);
        moveFront(bList);

        while (1){
            Entry aEnt = get(aList);
            Entry bEnt = get(bList);
            if (get(aList) == (void*) -1|| get(bList) == (void*) -1){
                break;
            }
            if (aFin == -1 && bFin == -1){
                break;
            }
            if (aFin != -1 && bFin == -1){
                changeEntry(M, i, aEnt->j, aEnt->value);
                aFin = moveOver(aList);
                continue;
            }
            if (aFin == -1 && bFin != -1){
                changeEntry(M, i, bEnt->j, 0 - bEnt->value);
                bFin = moveOver(bList);
                continue;
            }
            if (aEnt->j == bEnt->j){
                changeEntry(M, i, aEnt->j, aEnt->value - bEnt->value);
                aFin = moveOver(aList);
                bFin = moveOver(bList);
                continue;
            }
            if (aEnt->j > bEnt->j){
                changeEntry(M, i, bEnt->j, 0 - bEnt->value);
                bFin = moveOver(bList);
                continue;
            }
            if (aEnt->j < bEnt->j){
                changeEntry(M, i, aEnt->j, aEnt->value);
                aFin = moveOver(aList);
                continue;
            }
        }
    }
    return M;
}


double listCruncher(List AA, List B){
    double total = 0.0;
    int aMove = 0;
    int bMove = 0;
    moveFront(AA);
    moveFront(B);
    Entry aEnt, bEnt;
    while (aMove != -1 && bMove != -1) {
        if (get(AA) == (void*) -1|| get(B) == (void*) -1){
            break;
        }
        aEnt = get(AA);
        bEnt = get(B);

        if (aEnt->j == bEnt->j) {
            total += aEnt->value * bEnt->value;
            aMove = moveOver(AA);
            bMove = moveOver(B);
        }
        if (aEnt->j < bEnt->j) {
            aMove = moveOver(AA);
        }
        if (aEnt->j > bEnt->j) {
            bMove = moveOver(B);
        }
    }
    return total;
}



// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    if (size(A) != size(B)){
        return A;
    }
    double output;
    Matrix M = newMatrix(size(A));
    Matrix newB = transpose(B);
    for (int i = 1; i < size(A)+1; ++i) {
        for (int j = 1; j < size(newB)+1; ++j) {
            List aList = A->array[i];
            List bList = newB->array[j];
            output = listCruncher(bList, aList);
            changeEntry(M, i, j, output);
        }
    }

    return M;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
    for (int i = 1; i < M->size+1; ++i) {
        List L = M->array[i];
        if (length(L) != 0) {
            fprintf(out, "%d:", i);
            moveFront(L);
            for (int j = 0; j < length(L); ++j) {
                Entry G = get(L);
                fprintf(out, " (%d, %.1lf)", G->j, G->value);
                if (index(L) + 1 < length(L)) {
                    moveNext(L);
                }
            }
            fprintf(out, "\n");
        }
    }
    return;
}

void printRow(FILE* out, List L, int i){
    fprintf(out, "RowPrint %d", i);
    moveFront(L);
    Entry G;
    for (int j = 0; j < length(L); ++j) {
        G = get(L);
        fprintf(out, " (%d, %.1lf)", G->j, G->value);
        if (index(L)+1 < length(L)){
            moveNext(L);
        }
    }

    fprintf(out, "\n");
}

void printEntry(FILE* out, Entry E){
    fprintf(out, "(%d, %.1f)\n", E->j, E->value);
}
