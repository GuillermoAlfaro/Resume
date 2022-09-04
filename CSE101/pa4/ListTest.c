#include "List.h"
#include "entry.h"
#include <stdio.h>
#include <stdlib.h>

void test(){
    List L = newList();
    append(L, 45);
    append(L, 5);

    printList(stdout, L);
    moveFront(L);
    printf("%d\n", get(L));

}

void secondTest(){
    Entry E = newEntry(1, 4);
    List L = newList();
    append(L, E);
    append(L, E);
    append(L, E);
    append(L, E);
    append(L, E);
    append(L, E);
    append(L, E);
    append(L, E);
    printRow(stdout, L, 1);


}

int main(){
    //test();
    //secondTest();
    return 0;
}