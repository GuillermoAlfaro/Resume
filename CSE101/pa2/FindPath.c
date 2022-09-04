//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA2
//************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
        addEdge(G,u, v);
    }
    printGraph(out, G);
    fprintf(out, "\n");
    deleteFront(L);
    deleteFront(L);
    moveFront(L);
    List path = newList();
    int source = front(L);
    deleteFront(L);
    moveFront(L);
    int destination = front(L);
    while (1){
        deleteFront(L);
        moveFront(L);
        BFS(G, source);
        getPath(path, G, destination);
        if (getDist(G, destination) == -69){
            fprintf(out, "The distance from %d to %d is infinity\n",
                    source, destination);
            fprintf(out, "No %d-%d path exists",source, destination);
        } else {
            fprintf(out, "The distance from %d to %d is %d\n",
                    source, destination, getDist(G, destination));
            fprintf(out, "A shortest %d-%d path is: ", source, destination);
            printList(out, path);
        }
        source = front(L);
        deleteFront(L);
        moveFront(L);
        destination = front(L);
        if (source == 0 || destination == 0){
            break;
        }
        fprintf(out, "\n\n");
        clear(path);
    }
    freeList(&L);
    freeList(&path);
    freeGraph(&G);
    // close files
    fclose(in);
    fclose(out);

    return(0);
}