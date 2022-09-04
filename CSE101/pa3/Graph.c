//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA2
//************************************************
#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UNDEF -100000
#define NIL -1

// Structs -------------------------------------------------------------------------
typedef struct Graph{
    List *neighborsVertx;   // Adjacency list
    int order;              // Number of vertices
    int size;               // Number of edges
    int *color;             // 0 = White, 1 = Gray, 2 = Black
    int *p;                // parent of v in BFS tree
    int *d;
    int *f;
    bool hasBFSBeenCalled;
} GraphObj;

/*** Constructors-Destructors ***/
//  Returns a Graph having n vertices and no edges
Graph newGraph(int n){
    n += 1;             // Done so indices 1 through n can be used
    Graph G = malloc(sizeof(GraphObj));
    G->p = (int *) malloc(n * sizeof(int));
    G->d = (int *) malloc(n * sizeof(int));
    G->f = (int *) malloc(n * sizeof(int));
    G->color = (int *) malloc(n * sizeof(int));
    G->neighborsVertx = (List *) malloc(n * sizeof(List));
    for (int i = 1; i < n; ++i) {
        G->neighborsVertx[i] = newList();
    }
    G->order = n;
    G->size = 0;
    G->hasBFSBeenCalled = false;
    return G;
}
// Frees all heap memory associated with graph pG
// Pre: pG and *pG not null
void freeGraph(Graph* pG){
    if (pG != NULL && *pG != NULL){
        for (int i = 1; i < (*pG)->order; ++i) {
            freeList(&(*pG)->neighborsVertx[i]);
            (*pG)->neighborsVertx[i] = NULL;
        }
        free((*pG)->neighborsVertx);
        (*pG)->neighborsVertx = NULL;
        free((*pG)->color);
        (*pG)->color = NULL;
        free((*pG)->p);
        (*pG)->p = NULL;
        free((*pG)->d);
        (*pG)->d = NULL;
        free((*pG)->f);
        (*pG)->f = NULL;
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/
// Returns number of vertices
int getOrder(Graph G){
    return G->order;
}

// Returns size of graph.
int getSize(Graph G){
    return G->size;
}

// Returns parent of vertex
// Pre: BFS has to have been called.
int getParent(Graph G, int u){
    if (G->hasBFSBeenCalled && 1 <= u && u <= getOrder(G)){
        return G->p[u];
    } else {
        return NIL;
    }
}
/* Pre: 1 <= u <= getOrder(G) */
int getDiscover(Graph G, int u){
    if (u >= 1 && u <= getOrder(G)){
        return G->d[u];
    } else {
        return UNDEF;
    }
}

/* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u){
    if (u >= 1 && u <= getOrder(G)){
        return G->f[u];
    } else {
        return UNDEF;
    }
}


/*** Manipulation procedures ***/
// Inserts u into L sorted from least to greatest.
void sort_insert(List L, int u){
    moveFront(L);
    if (get(L) != -1){
        for (int i = 0; i < length(L); ++i) {
            if (get(L) > u){
                insertBefore(L, u);
                break;
            } else if (get(L) == back(L)){
                insertAfter(L, u);
                break;
            } else {
                moveNext(L);
            }
        }
    } else {
        append(L, u);
    }
}

// Returns graph G back to its original created state.
void makeNull(Graph G){
    for (int i = 1; i < G->order; ++i) {
        clear(G->neighborsVertx[i]);
        G->p[i] = 0;
        G->d[i] = 0;
        G->color[i] = 0;
        G->hasBFSBeenCalled = false;
    }

}
// Adds u and v to each of their neighborsVertex list
// Pre: u and v between 1 to getOrder(G)
void addEdge(Graph G, int u, int v){
    G->size += 1;
    if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)){
        return;
    }
    sort_insert(G->neighborsVertx[u], v);
    sort_insert(G->neighborsVertx[v], u);
}
// Adds single direction lane from U -> V
// Pre: u and v between 1 to getOrder(G)
void addArc(Graph G, int u, int v){
    G->size += 1;
    if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)){
        return;
    }
    sort_insert(G->neighborsVertx[u], v);
}
void Visit(Graph G, int x, int time, List *s){
    time = time + 1;
    G->d[x] = time;                     // time x was discovered
    G->color[x] = 1;                    // colors x to grey
    List L = G->neighborsVertx[x];
    for (moveFront(L); index(L)>= 0; moveNext(L)) {
        int y = get(L);
        if (G->color[y] == 0){
            G->p[y] = x;
            Visit(G, y, time, s);
        }
        if (length(L) == 1){

            break;
        }
    }
    G->color[x] = 2;
    G->f[x] = time + 1;
    prepend(*s, x);
}


/* Pre: length(S)==getOrder(G) */
void DFS(Graph G, List s){
    G->hasBFSBeenCalled = true;
    List sCopy = copyList(s);
    clear(s);
    if (length(sCopy) + 1 != getOrder(G)){return;}
    for (moveFront(sCopy); index(sCopy)>=0; moveNext(sCopy)){
        int x = get(sCopy);
        G->color[x] = 0;        // color[x] = white
        G->p[x] = NIL;
    }
    int time = 0;
    for (moveFront(sCopy); index(sCopy)>=0; moveNext(sCopy)){   // main loop of DFS
        int x = get(sCopy);
        if (G->color[x] == 0) {
            Visit(G, x, time, &s);
        }
    }
    freeList(&sCopy);
}

/*** Other operations ***/
Graph transpose(Graph G){
    Graph D = newGraph(getOrder(G) - 1);
    for (int i = 1; i < getOrder(G); ++i) {
        moveFront(G->neighborsVertx[i]);
        for (int j = 1; j < 1 + length(G->neighborsVertx[i]); ++j) {
            int rightSide = get(G->neighborsVertx[i]);
            int leftSide = i;
            sort_insert(D->neighborsVertx[rightSide], leftSide);
            if (length(G->neighborsVertx[i]) > j) {
                moveNext(G->neighborsVertx[i]);
            }
        }
    }
    return D;

}

Graph copyGraph(Graph G){
    Graph D = newGraph(getOrder(G) - 1);
    for (int i = 1; i < getOrder(G); ++i) {
        moveFront(G->neighborsVertx[i]);
        for (int j = 1; j < 1 + length(G->neighborsVertx[i]); ++j) {
            sort_insert(D->neighborsVertx[i], get(G->neighborsVertx[i]));
            if (length(G->neighborsVertx[i]) > j) {
                moveNext(G->neighborsVertx[i]);
            }
        }
    }
    return D;
}

void printGraph(FILE* out, Graph G){
    for (int i = 1; i < G->order; ++i) {
        fprintf(out, "%d: ", i);
        printList(out, G->neighborsVertx[i]);
        fprintf(out, "\n");
    }

}