//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA2
//************************************************
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

#define UNDEF -100000
#define NIL -1


typedef struct Graph* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);

/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u);      /* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u);    /* Pre: 1 <= u <= getOrder(G) */
int getFinish(Graph G, int u);      /* Pre: 1<=u<=n=getOrder(G) */


/*** Manipulation procedures ***/
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void DFS(Graph G, List s);          /* Pre: length(S)==getOrder(G) */

/*** Other operations ***/
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out, Graph G);