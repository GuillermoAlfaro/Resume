#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Stack Stack;

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};
// Initializer for stuct stacks
Stack *stack_create(uint32_t capacity) {
    Stack *gum = (Stack *) malloc(sizeof(Stack));
    gum->items = (Node **) malloc(capacity * sizeof(Node));
    gum->capacity = capacity;
    gum->top = 0;
    return gum;
}
// Deconstructor for stack, frees poitner and
// assigns them to NULL
void stack_delete(Stack **s) {
    if (*s) {
        free((*s)->items);
        (*s)->items = NULL;
        free(*s);
        *s = NULL;
    }
}
// Returns true if stack is empty
// false otherwise
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}
// Returns true if stack is full
// false otherwise
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}
// Returns stack size
uint32_t stack_size(Stack *s) {
    return s->top;
}
// Adds node into stack and increments
// top to show that
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
}
// Removes node from top of stack
// and decrements top
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top -= 1;
    *n = s->items[s->top];
    return true;
}
// Debug fucnction to visualize stack
void stack_print(Stack *s) {
    for (int i = 0; i < s->top; ++i) {
        node_print(s->items[i]);
    }
}
