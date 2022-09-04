#include "node.h"
#include "code.h"
#include "defines.h"
#include "pq.h"
#include "io.h"
#include "stack.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// Given an array that has ASCII spaces
// creates a PQ with each ASCII character and their frequency
// then performs huffman processing to combine them into single root node
// returns root node
Node *build_tree(uint64_t hist[static ALPHABET]) {
    int counter = 0;
    for (int i = 0; i < ALPHABET; ++i) {
        // Only is there is something in the ASCII space will there be space allocated for it
        if (hist[i] > 0) {
            ++counter;
        }
    }
    // Creates PQ with amount of non-zero frequencies
    PriorityQueue *var = pq_create(counter);
    // Creates placeholder left and right node initialized to 0
    Node *left = node_create(0, 0);
    Node *right = node_create(0, 0);
    // Creates node for all non zero frequecies
    // then enqueues
    for (int i = 0; i < ALPHABET; ++i) {
        if (hist[i] != 0) {
            Node *sigma = node_create(i, hist[i]);
            enqueue(var, sigma);
        }
    }
    Node *parent;
    // Dequeues a left and right node
    // combines them and enqueus them again
    // till 1 root node is left
    while (pq_size(var) > 1) {
        dequeue(var, &left);
        dequeue(var, &right);
        parent = node_join(left, right);
        enqueue(var, parent);
    }
    Node *root;
    dequeue(var, &root);
    // return root node
    return root;
}

static Code c; // static varible Code C so it doesn't get recreated every recurse of build_codes()
static uint8_t bit_popped; // place holder uint8_t varible to push and pop

// Traverses down the root node enqueuing a 0
// or a left and 1 for a right
// when reached a leaf, takes the symbol and code and adds
// it to Code table
void build_codes(Node *root, Code table[static ALPHABET]) {
    // Since static varible have to be known at combile time
    // work around checks if has been initialized and if not
    // initialized Code c
    if (c.top == 0) {
        c = code_init();
    }
    // Traverse only if there is a starting point
    if (root != NULL) {
        // If leaf has been found goes into this if
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
            // else enters else recurse
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            // removes "breadcrumb trail" to indicate searching for new leaf
            code_pop_bit(&c, &bit_popped);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit_popped);
        }
    }
}
// Writes out "breadcrumb trail" using L for leaf and I for interior
// in post-order traversal
void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        // If there is no more left or right paths
        // write L and symbol of leaf node
        if (root->left == NULL && root->right == NULL) {
            write(outfile, "L", 1);
            char def = root->symbol;
            write(outfile, &def, 1);
        } else {
            write(outfile, "I", 1);
        }
    }
}
// Rebuilds tree from dump_tree
// length of bytes of dump is from nbytes
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    // Creats stack of nodes to assign all contain nodes
    // does reverse of dump tree while building using build_tree
    // to singular root node
    Stack *nodes = stack_create(nbytes);
    Node *var = node_create(0, 0);
    Node *left;
    Node *right;
    Node *parent;
    int i = 0;
    while (i <= nbytes) {
        if (tree[i] == 'L') {
            var->symbol = tree[i + 1];
            stack_push(nodes, var);
        }
        if (tree[i] == 'I') {
            stack_pop(nodes, &left);
            stack_pop(nodes, &right);
            parent = node_join(left, right);
            stack_push(nodes, parent);
        }
        i++;
    }
    return var;
}
// Deconstructs tree freeing all pointers
// and assigning them to NULL
void delete_tree(Node **root);
