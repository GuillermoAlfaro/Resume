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

#define INF -69
#define NIL -1

// Structs -------------------------------------------------------------------------
typedef struct Graph{
    List *neighborsVertx;   // Adjacency list
    int order;              // Number of vertices
    int size;               // Number of edges
    int source;             // Most recent vertices
    int *color;             // 0 = White, 1 = Gray, 2 = Black
    int *p;                // parent of v in BFS tree
    int *d;                // distance from source to vertex
    bool hasBFSBeenCalled;
} GraphObj;

/*** Constructors-Destructors ***/
//  Returns a Graph having n vertices and no edges
Graph newGraph(int n){
    n += 1;             // Done so indices 1 through n can be used
    Graph G = malloc(sizeof(GraphObj));
    G->p = (int *) malloc(n * sizeof(int));
    G->d = (int *) malloc(n * sizeof(int));
    G->color = (int *) malloc(n * sizeof(int));
    G->neighborsVertx = (List *) malloc(n * sizeof(List));
    for (int i = 1; i < n; ++i) {
        G->neighborsVertx[i] = newList();
    }
    G->order = n;
    G->size = 0;
    G->source = NIL;
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
// Returns most recent BFS number called
// Pre: BFS had to be called.
int getSource(Graph G){
    return G->source;
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
int getDist(Graph G, int u){
    if (G->hasBFSBeenCalled && 1 <= u && u <= getOrder(G)){
        return G->d[u];
    } else {
        return INF;
    }
}

// Find the shortest path between source vertex and U
// Pre: getSource(G) != NULL and a <= u <= getOrder(G)
void getPath(List L, Graph G, int u){
    if (G->hasBFSBeenCalled && 1 <= u && u <= getOrder(G)){
        if (G->source == u){
            append(L, u);
        } else if (G->p[u] == NIL){
            append(L, NIL);
        } else {
            getPath(L, G, G->p[u]);
            append(L, u);
        }
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
        G->source = NIL;
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
// Adds
// Pre: u and v between 1 to getOrder(G)
void addArc(Graph G, int u, int v){
    if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)){
        return;
    }
    sort_insert(G->neighborsVertx[u], v);
}
void BFS(Graph G, int s){
    G->hasBFSBeenCalled = true;
    G->source = s;
    if (s < 1 || s > getOrder(G)){
        return;
    }
    for (int i = 1; i < getOrder(G); ++i) {
        G->color[i] = 0;            // Sets color to 0 (White)
        G->d[i] = INF;
        G->p[i] = NIL;
    }
    G->color[s] = 1;                // Sets color to 1 (Gray)
    G->d[s] = 0;
    G->p[s] = NIL;
    List Q = newList();
    append(Q, s);
    moveFront(Q);
    while (length(Q) > 0){
        int x = get(Q);
        deleteFront(Q);
        int counter = 0;
        moveFront(G->neighborsVertx[x]);
        while (counter < length(G->neighborsVertx[x])) {
            counter += 1;
            int y = get(G->neighborsVertx[x]);
            if (G->color[y] == 0){          // If y is undiscovered
                G->color[y] = 1;            // Discover y
                G->d[y] = G->d[x] + 1;
                G->p[y] = x;
                append(Q, y);
            }
            if (counter < length(G->neighborsVertx[x])) {
                moveNext(G->neighborsVertx[x]);
            }
        }
        moveFront(Q);
        G->color[x] = 2;                    // finish x
    }
    freeList(&Q);
}

/*** Other operations ***/
void printGraph(FILE* out, Graph G){
    for (int i = 1; i < G->order; ++i) {
        fprintf(out, "%d: ", i);
        printList(out, G->neighborsVertx[i]);
        fprintf(out, "\n");
    }
}