//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA4
//************************************************
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct List* List;
typedef void* ListElement;


// Constructors-Destructors
List newList(void);
void freeList(List *pL);


// Access functions
int length(List L);
int index(List L);
ListElement front(List L);
ListElement back(List L);
void * get(List L);
bool equalz(List A, List B);


// Manipulation procedures
void clear(List L);
void set(List L, ListElement x);

void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, ListElement x);
void append(List L, ListElement x);
void insertBefore(List L, ListElement x);
void insertAfter(List L, ListElement x);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);


// Other operations
void printList(FILE* out, List L);
List copyList(List L);
List concatList(List A, List B);
