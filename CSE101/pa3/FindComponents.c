//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA3
//************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <string.h>

#include "List.h"
#include "Graph.h"

#define MAX_LEN 300


int main(int argc, char * argv[]){
    bool lever = false;
    FILE *in, *out;
    char line[MAX_LEN];

    // check command line for correct number of arguments
    if( argc != 3 ){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    if( in==NULL ){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    List L = newList();
    int xx = strtol(fgets(line, MAX_LEN, in), NULL, 10);
    Graph G = newGraph(xx);
    while (1) {
        if(fgets(line, MAX_LEN, in) == NULL){
            break;
        }
        int u, v;
        sscanf(line, "%d %d", &u, &v);
        if ((u == 0 && v == 0) || lever == true){
            lever = true;
            append(L, u);
            append(L, v);
            continue;
        }
        addArc(G,u, v);
    }
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");
    List s = newList();
    for (int i = 1; i < xx+1; ++i) {
        append(s, i);
    }
    DFS(G, s);
    G = transpose(G);
    DFS(G, s);
    moveFront(s);
    int counter = 0;

    for (int i = 0; i < length(s); ++i) {
        if (getParent(G, get(s)) == -1){
            counter += 1;
        }
        moveNext(s);
    }
    moveFront(s);
    fprintf(out, "G contains %d strongly connected components:", counter);
    counter = 0;
    List stack = newList();
    for (moveBack(s); index(s)>=0; movePrev(s)){
        if (getParent(G, get(s)) != -1){
            prepend(stack, get(s));
        } else {
            prepend(stack, get(s));
            counter += 1;
            fprintf(out, "\nComponent %d: ", counter);
            printList(out, stack);
            clear(stack);
        }

    }
    freeList(&stack);
    freeList(&s);
    freeList(&L);
    freeGraph(&G);
    // close files
    fclose(in);
    fclose(out);

    return(0);
}