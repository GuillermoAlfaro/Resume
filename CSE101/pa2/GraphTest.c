//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA2
//************************************************
#include "Graph.h"
#include "List.h"

#include <stdlib.h>
#include <stdio.h>

int create_graph(){
    Graph G = newGraph(10);
    freeGraph(&G);
    return 0;
}

int lists_of_lists(){
    List *L =  (List *) malloc(4 * sizeof(List));
    for (int i = 0; i < 4; ++i) {
        L[i] = newList();
        append(L[i], 5);
        append(L[i], 7);
        append(L[i], 9);
    }
    for (int i = 0; i < 4; ++i) {
        printf("\nLIST L[%d]\n", i);
        printList(stdout, L[0]);
    }
    return 0;

}

int array_of_array(){
    int **arr = (int **) malloc(sizeof(int*) * 3);
    for (int i = 0; i < 3; ++i) {
        arr[i] = (int *) malloc(sizeof (int));
        arr[i][0] = i + 2;
    }
    for (int i = 0; i < 3; ++i) {
        printf("arr[%d][0] = %d\n", i, arr[i][0]);
    }
    return 0;
}

int test(){
    List L = newList();
    append(L, 5);

    printList(stdout, L);


    return 0;
}

void print_graph(){
    Graph G = newGraph(5);

    addEdge(G,1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 1, 3);
    addEdge(G,1, 5);
    addArc(G,3, 2);
    addEdge(G,2, 4);

    printGraph(stdout, G);

}

int BFS_test(){

    Graph G = newGraph(6);

    addEdge(G,1, 2);
    addEdge(G,2, 5);
    addEdge(G,2, 6);
    addEdge(G,3, 4);
    addEdge(G,3, 6);
    addEdge(G,5, 6);

    BFS(G, 2);
    List L = newList();
    getPath(L, G, 4);
    BFS(G, 3);
    getPath(L, G, 2);
    printList(stdout, L);
    puts("");
    //makeNull(G);
    printGraph(stdout, G);

    return 0;

}

int list_dFront(){
    List L = newList();
    append(L, 5);
    append(L, 2);
    moveFront(L);
    printf("Cursor = %d\n", get(L));
    deleteFront(L);
    printf("Cursor = %d\n", get(L));
    //while (length(L) > 0) {
    //
    //
    //    deleteFront(L);
    //}
    return 0;
}

int NonEmptyback(){
    List A = newList();
    append(A, 5);
    prepend(A, 7);
    append(A, 2);
    moveBack(A);
    insertAfter(A, 43);
    deleteBack(A);
    delete (A);
    if (back(A) != 5)
        return 1;
    return 0;
}

int emptycoptlist(){
    List A = newList();
    List C = copyList(A);
    printList(stdout, A);
    puts("");
    printList(stdout, C);
    puts("");
    printf("Result of equals is %d\n", equals(A, C));
    bool res = (!equals(A, C) || A == C);

    freeList(&C);
    if (res)
        return 1;
    return 0;
}

int nonemptyopy(){
    List A = newList();
    append(A, 2);
    prepend(A, 1);
    moveFront(A);
    List C = copyList(A);
    printf("Index(A) = %d\n", index(A));                // 0
    if (A == C){
        puts("FAILED");
    }
    bool res = (index(A) != 0 || !equals(A, C) || A == C);
    printf("res = %d\n", res);
    freeList(&C);
    if (res)
        return 1;
    return 0;
}

int main(){
    create_graph();
    //lists_of_lists();
    //array_of_array();
    //test();
    //print_graph();
    //BFS_test();
    //list_dFront();

    return 0;
}