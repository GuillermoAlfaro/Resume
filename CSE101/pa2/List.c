//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA2
//************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "List.h"

// Structs --------------------------------------------------------------------------------
typedef struct NodeObj* Node;
typedef struct NodeObj {
    ListElement data;
    Node next;
    Node prev;
} NodeObj;

typedef struct List {
    Node front;
    Node back;
    Node cursor;
    ListElement length;
    ListElement curse;
} ListObj;

// Constructors-Destructors ---------------------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next prev and data fields.
Node newNode(ListElement data){
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->next = NULL;
    N->prev = NULL;
    return N;
}

// freeNode()
// Frees heap memory allocated in newNode().
void freeNode(Node *N){
    if (N != NULL && *N != NULL){
        free(*N);
        *N = NULL;
    }
}

// Returns reference to new empty List object.
List newList(void){
    ListObj *L = (ListObj *) malloc(sizeof(ListObj));
    L->front = L->back = NULL;
    L->cursor = NULL;
    L->curse = -1;
    L->length = 0;
    return L;
}


void freeList(List *pL){
    if (pL != NULL && *pL != NULL) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}


// Access functions -----------------------------------------------------------------------
int length(List L){
    return L->length;
}
// Returns index length of L
int index(List L){
    if (L != NULL){
        return L->curse;
    } else {
        return -1;
    }
}
// Returns front length of L
// Pre: length(L) > 0
int front(List L){
    if (length(L) > 0){
        return L->front->data;
    } else {
        return -1;
    }
}
// Returns back element of L
// Pre: length(L) > 0
int back(List L){
    if (length(L) > 0){
        return L->back->data;
    } else {
        return -1;
    }
}

int get(List L){
    if (length(L) > 0 && L->curse >= 0){
        return L->cursor->data;
    } else {
        return -1;
    }
}
bool equals(List A, List B){
    if (A->length != B->length){
        return false;
    }
    if (A->length == 0){
        return true;
    }
    int tempCurse = A->curse;
    moveFront(A);
    moveFront(B);
    int counter = 0;
    while (A->curse < A->length){
        int a = get(A);
        int b = get(B);
        if (a != b){
            moveFront(A);
            moveFront(B);
            for (int i = 0; i < tempCurse; ++i) {
                moveNext(A);
                moveNext(B);
            }
            return false;
        }
        counter += 1;
        if (counter == A->length){
            return true;
        }
        moveNext(A);
        moveNext(B);
    }
    return true;
}


// Manipulation procedures ----------------------------------------------------------------
void clear(List L){
    moveBack(L);
    Node tempNode;
    while (L->length > 0){
        tempNode = L->cursor->prev;
        freeNode(&L->cursor);
        L->cursor = tempNode;
        L->curse -= 1;
        L->length -= 1;
    }
}
// Overwrites the cursor element's data with x.
// Pre: length() > 0 && index() > 0
void set(List L, int x){
    if (L->length > 0 && index(L) > 0){
        L->cursor->data = x;
    }
}
// Moves Cursor to front of list.
// Pre: L is not empty otherwise does nothing.
void moveFront(List L){
    if (L->length == 0){
        return;
    }
    L->curse = 0;
    L->cursor = L->front;
}
void moveBack(List L){
    if (L->length == 0){
        return;
    }
    L->curse = L->length - 1;
    L->cursor = L->back;
}
void movePrev(List L){
    if (L->cursor != NULL){
        if (L->cursor != L->front){
            L->cursor = L->cursor->prev;
            L->curse -= 1;
        } else {
            Node temp = L->cursor->next;
            Node newtemp = L->cursor;
            L->cursor = NULL;
            temp->prev = newtemp;
            L->curse = -1;
        }
    }


}
// Moves cursor to next node
// If cursor is defined at the back, it becomes undefined
// If Cursor is undefined do nothing
void moveNext(List L){
    if (L->cursor != NULL){
        if (L->cursor != L->back){
            L->cursor = L->cursor->next;
            L->curse += 1;
        } else {
            Node temp = L->cursor->prev;
            Node newtemp = L->cursor;
            L->cursor = NULL;
            temp->next = newtemp;
            L->curse = -1;
        }
    }
}
void prepend(List L, ListElement x){
    Node app = newNode(x);
    if (L->length == 0){
        L->front = app;
        L->back = app;
        L->length += 1;
    } else {
        L->front->prev = app;
        app->next = L->front;
        L->front = app;
        L->length += 1;
    }
    if (L->cursor >= 0){
        L->curse += 1;
    }
}
void append(List L, ListElement x){
    Node app = newNode(x);
    if (L->length == 0){
        L->front = app;
        L->back = app;
        L->length += 1;
    } else {
        L->back->next = app;
        app->prev = L->back;
        L->back = app;
        L->length += 1;
    }
}
// Insert new elements before cursor
// Pre: length()>0 && index()>0
void insertBefore(List L, ListElement x){
    if (L->length > 0 && L->curse>=0){
        Node app = newNode(x);
        if (L->curse == 0){
            L->front = app;
        } else {
            L->cursor->prev->next = app;
        }
        app->prev = L->cursor->prev;
        L->cursor->prev = app;
        app->next = L->cursor;
        L->length += 1;
        L->curse += 1;
        return;
    }
}
// Insert new elements after cursor
// Pre: length()>=0 && index()>=0
void insertAfter(List L, ListElement x){
    if (L->length > 0 && L->cursor == L->back){
        append(L, x);
        return;
    }
    if (L->length > 0 && L->curse>=0){
        Node app = newNode(x);
        L->cursor->next->prev = app;
        app->next = L->cursor->next;
        app->prev = L->cursor;
        L->cursor->next = app;
        L->length += 1;
        return;
    }
}
// Deletes the front element
// Pre: length()>0
void deleteFront(List L){
    if (L->length > 0){
        Node temp = L->front->next;
        if (L->length != 1) {
            L->front->next->prev = NULL;
            L->front->next = NULL;
        }
        freeNode(&L->front);
        L->front = temp;
        L->length -= 1;
        if (L->length == 1){
            L->back = L->front;
        }
        if (L->cursor != NULL && L->curse >= 0){
            L->curse -= 1;
        }
    }
}
// Deletes the back element
// Pre: length()>0
void deleteBack(List L){
    if (L->length > 0){
        if (L->cursor == L->back){
            L->cursor = NULL;
            L->curse = -1;
        }
        Node temp = L->back->prev;
        if (L->length > 1){
            L->back->prev->next = NULL;
        }
        L->back->prev = NULL;
        freeNode(&L->back);
        L->back = temp;
        L->length -= 1;
        if (L->curse >= L->length){
            L->curse -= 1;
        }
        if (L->length == 1){
            L->front = L->back;
        }
        if (L->length == 0){
            L->front = NULL;
        }
    }
}
// Deletes the cursor element
// Pre: length()>0 && index()>0
void delete(List L){
    if (L->length > 0 && L->curse >= 0){
        if (L->cursor == L->front){
            deleteFront(L);
            return;
        }
        if (L->cursor == L->back){
            deleteBack(L);
            return;
        }
        L->cursor->next->prev = L->cursor->prev;
        L->cursor->prev->next = L->cursor->next;
        L->cursor->next = NULL;
        L->cursor->prev = NULL;
        freeNode(&L->cursor);
        L->curse = -1;
        L->length -= 1;

    }

}


// Other operations -----------------------------------------------------------------------

void printList(FILE* out, List L){
    moveFront(L);
    int counter = 0;
    while (index(L) >= 0){
        fprintf(out, "%d ", get(L));
        if (L->length == 1){
            break;
        }
        moveNext(L);
        counter += 1;
        if (counter == L->length){
            break;
        }
    }
}
List copyList(List L){
    int position = L->curse;
    List new = newList();
    if (L->length == 0){
        return new;
    }
    moveFront(L);
    int counter = 0;
    while (1){
        int temp = get(L);
        append(new, temp);
        counter += 1;
        if (counter == L->length){
            break;
        }
        moveNext(L);
    }
    if (position == 0){
        moveFront(L);
    }
    return new;
}
List concatList(List A, List B){
    moveFront(B);
    while (1){
        int data = get(B);
        append(A, data);
        if (length(B)-1 == index(B)){
            break;
        }
        moveNext(B);
    }
    return A;
}
