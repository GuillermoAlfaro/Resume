#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node Node;

struct Node {
    Node *left;
    Node *right;
    uint8_t symbol;
    uint64_t frequency;
};
// Initialized Node
Node *node_create(uint8_t symbol, uint64_t frequency) { // Leaf nodes created here
    Node *var = (Node *) malloc(sizeof(Node));
    var->symbol = symbol;
    var->frequency = frequency;
    var->left = NULL;
    var->right = NULL;
    return var;
}

// Frees then assigns to NULL
void node_delete(Node **n) {
    if (*n) {
        free((*n)->left);
        (*n)->left = NULL;
        free((*n)->right);
        (*n)->right = NULL;
        free(*n);
        *n = NULL;
    }
    return;
}

Node *node_join(Node *left, Node *right) { // Parent nodes created here
    uint64_t joined_freq = left->frequency + right->frequency;
    Node *joined = node_create('$', joined_freq);
    joined->right = right;
    joined->left = left;
    return joined;
}
// Debug function to see node and their children if any
void node_print(Node *n) {
    printf("Node symbol %c\n", n->symbol);
    printf("Node frequency %lu\n\n", n->frequency);
    if (n->left != NULL) {
        puts("LEFT");
        node_print(n->left);
    }
    if (n->right != NULL) {
        puts("RIGHT");
        node_print(n->right);
    }
}
