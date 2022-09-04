#include "List.h"
#include <stdio.h>
#include <stdlib.h>

int lister(){
    List L = newList();
    append(L, 1);
    append(L, 2);
    append(L, 3);
    moveBack(L);
    insertBefore(L, 10);
    append(L, 4);
    prepend(L, 0);
    insertAfter(L, 55);
    deleteFront(L);


    printf("Cursor of L = %d and its position = %d\n", get(L), index(L));
    printf("Front of L = %d\n", front(L));
    printf("Back of L = %d\n", back(L));
    printf("Length of L = %d\n", length(L));


    printList(stdout, L);
    puts("\nL has been Freeded");
    freeList(&L);


    return 0;
}

int freeTest(){

    List L = newList();
    append(L, 5);
    append(L, 20);
    append(L, 51);
    append(L, 51);
    append(L, 51);

    List B = newList();
    append(B, 5);
    append(B, 20);
    append(B, 50);
    append(B, 51);
    append(B, 51);
    printf("List L: ");
    printList(stdout, L);
    printf("\nList B: ");
    printList(stdout, B);
    //printf("\nFront = %d\nBack = %d\n", front(L), back(L));
    puts("");
    printf("Equal: %d\n", equals(L, B));
    freeList(&L);
    freeList(&B);
    return 0;
}

int coptTest(){

    List L = newList();
    append(L, 45);
    append(L, 1);
    append(L, 231);
    List G = copyList(L);
    printf("List L: ");
    printList(stdout, L);
    printf("\nList G: ");
    printList(stdout, G);
    puts("");
    freeList(&L);
    freeList(&G);

    return 0;
}

int insertbefore_length(){
    List L = newList();
    append(L, 1);
    append(L, 1);
    moveFront(L);
    insertBefore(L, 50);
    printList(stdout, L);
    freeList(&L);
    return 0;
}

int delteback_length(){
    List L = newList();
    //append(L, 5);
    //append(L, 10);
    append(L, 12);
    deleteBack(L);
    printList(stdout, L);
    freeList(&L);

    return 0;
}

int delete_length(){
    List L = newList();
    append(L, 1);
    append(L, 2);
    append(L, 3);
    moveFront(L);
    delete(L);
    printList(stdout, L);
    freeList(&L);
    return 0;
}

int move_next_index(){
    List L = newList();
    append(L, 1);
    append(L, 2);
    append(L, 3);
    append(L, 4);
    moveBack(L);
    moveNext(L);
    //printf("Cursor value = %d\n", get(L));
    //printList(stdout, L);
    puts("");
    append(L, 5);
    printList(stdout, L);
    freeList(&L);


    return 0;
}

int insertAfterIndex(){

    List L = newList();
    append(L, 1);
    append(L, 2);
    append(L, 3);
    append(L, 4);
    moveBack(L);
    insertAfter(L, 5);
    printList(stdout, L);
    freeList(&L);

    return 0;
}

int delete_front_index(){
    List A = newList();
    prepend(A, 5);
    prepend(A, 65);
    prepend(A, 43);
    prepend(A, 2);
    prepend(A, 8);
    prepend(A, 1);
    moveFront(A);
    deleteFront(A);
    if (index(A) != -1) {
        printList(stdout, A);
        printf("Index is %d\n", index(A));
        return 1;
    }
    moveBack(A);
    deleteFront(A);
    if (index(A) != 3)
        return 2;
    return 0;
}

int delete_back_index(){
    List A = newList();
    prepend(A, 5);
    prepend(A, 65);
    prepend(A, 43);
    prepend(A, 2);
    prepend(A, 8);
    prepend(A, 1);
    moveBack(A);
    deleteBack(A);
    if (index(A) != -1) {
        printf("A->Curse = %d\n", index(A));
        return 1;
    }
    moveFront(A);
    deleteBack(A);
    moveNext(A);
    if (index(A) != 1) {
        printf("A->Curse = %d\n", index(A));
        return 2;
    }
    return 0;
}

int insertAfter_equals(){
    List A = newList();
    List B = newList();
    append(A, 1);
    append(B, 1);
    append(A, 2);
    moveFront(B);
    insertAfter(B, 2);
    if (!equals(A, B))
        return 1;
    append(B, 3);
    moveBack(A);
    insertAfter(A, 3);
    if (!equals(A, B))
        return 2;
    return 0;
}

int main(){
    //lister();
    //freeTest();
    //coptTest();
    //insertbefore_length();
    //delteback_length();
    //delete_length();
    //move_next_index();
    //insertAfterIndex();
    //delete_back_index();
    int g = insertAfter_equals();
    if (g == 1){
        puts("A and B are not equal (1)");
    }
    if (g == 2){
        puts("A and B are not equal (2)");
    }

    return 0;
}