#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
    char *word;
    uint32_t count;
};
// Allocates space for a Node
Node *node_create(char *word) {

    Node *var = (Node *) malloc(sizeof(Node));
    // Copies the word pointers and returns one of equal size
    var->word = strdup(word);
    var->count = 0;
    return var;
}
// Frees all pointers and sets them to NULL
void node_delete(Node **n) {
    free((*n)->word);
    (*n)->word = NULL;
    free(*n);
    *n = NULL;
}
// Debug function to see each Node
void node_print(Node *n) {
    printf("%s %d\n", n->word, n->count);
}
