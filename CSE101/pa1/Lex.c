#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEN 300

void before(List L, char **array, int pos){
    while (index(L) > 0){
        movePrev(L);
        int newPos = get(L);
        int comparison = strcmp(array[pos], array[newPos]);
        if (comparison >= 0){
            insertAfter(L, pos);
            return;
        }
    }
    prepend(L, pos);
}
void after(List L, char **array, int pos){
    while (index(L) < length(L)-1){
        moveNext(L);
        int newPos = get(L);
        int comparison = strcmp(array[pos], array[newPos]);
        if (comparison <= 0){
            insertBefore(L, pos);
            return;
        }
    }
    append(L, pos);
}

int main(int argc, char * argv[]){
    FILE *in, *out;
    char **array;
    int line_counter = 0, array_pointer = 0;
    char line[MAX_LEN];
    char buffer[MAX_LEN];
    List L = newList();


    // check command line for correct number of arguments
    if (argc != 3){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
    if (in == NULL){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if(out == NULL){
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    while(fgets(line, MAX_LEN, in) != NULL){
        line_counter += 1;
    }
    rewind(in);
    array = malloc(line_counter * sizeof(char *));
    while(fgets(buffer, MAX_LEN, in) != NULL){
        array[array_pointer] = malloc((strlen(buffer)) * sizeof(char));
        strcpy(array[array_pointer], buffer);
        array_pointer += 1;
    }

    append(L, 0);
    for (int i = 1; i < line_counter; ++i) {
        moveFront(L);
        int array_index = get(L);
        int comparison = strcmp(array[i], array[array_index]);
        if (length(L) > 1 && comparison < 0){
            before(L, array, i);
        } else if (length(L) > 1 && comparison > 0){
            after(L, array, i);
        } else {
            if (comparison < 0) {
                prepend(L, i);
            } else {
                append(L, i);

            }
        }
    }

    moveFront(L);
    while (1){
        int array_pos = get(L);
        fprintf(out, "%s ", array[array_pos]);
        if (length(L)-1 == index(L)){
            break;
        }
        moveNext(L);
    }
    fprintf(out, "\n");

    for (int i = 0; i < line_counter; ++i) {
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
    freeList(&L);
    // close files
    fclose(in);
    fclose(out);

    return(0);
}