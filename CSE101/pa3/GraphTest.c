#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "List.h"

void copyTest(){
    Graph G = newGraph(5);
    addArc(G, 2, 4);
    addArc(G, 4, 1);
    addArc(G, 1,  5);
    addEdge(G, 3, 4);

    printGraph(stdout, G);
    puts("");
    Graph D = copyGraph(G);
    printGraph(stdout, D);
}

void dfsTest(){
    Graph G = newGraph(8);
    addArc(G, 1, 2);
    addArc(G, 2, 5);
    addArc(G, 4, 1);
    addArc(G, 2, 6);
    addArc(G, 5, 7);
    addArc(G, 8, 5);
    addArc(G, 6, 5);
    addArc(G, 6, 3);
    addArc(G, 5, 4);


    //printGraph(stdout, G);
    //puts("");
    List s = newList();
    append(s, 1);
    append(s, 2);
    append(s, 3);
    append(s, 4);
    append(s, 5);
    append(s, 6);
    append(s, 7);
    append(s, 8);
    printList(stdout, s);
    puts("\nnew list");
    DFS(G, &s);
    printList(stdout, s);
}

void moveOffEdge(){
    List L = newList();
    append(L, 3);
    append(L, 4);
    append(L, 9);
    printList(stdout, L);
    puts("");
    for (moveFront(L); index(L)>=0; moveNext(L)){
        printf("Index(L) = %d\nget(L) = %d\n", index(L), get(L));
    }

}

int main(){
    //copyTest();
    //dfsTest();
    //moveOffEdge();
    return 0;
}