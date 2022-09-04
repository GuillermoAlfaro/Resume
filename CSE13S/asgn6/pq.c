#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue {
    uint32_t n;
    Node **arr; // Creates array of Nodes which is why ** is used
    uint32_t top;
};

// Min frequency at highest position Insertion sort
void insertion_sort(PriorityQueue *q) {
    for (uint32_t ii = 0; ii < q->top; ii++) {
        uint32_t jj = ii;
        Node *temp = q->arr[ii];
        while (jj > 0 && temp->frequency > (q->arr[jj - 1])->frequency) {
            q->arr[jj] = (q->arr[jj - 1]);
            jj -= 1;
        }
        q->arr[jj] = temp;
    }
}
// Initialized function to create space for priority queue and array
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc((sizeof(PriorityQueue)));
    pq->arr = (Node **) malloc(capacity * sizeof(Node));
    pq->n = capacity;
    pq->top = 0;
    return pq;
}
// Deconstructs priority queue
void pq_delete(PriorityQueue **q) {
    free((*q)->arr);
    (*q)->arr = NULL;
    free(*q);
    *q = NULL;
}
// Checks if PQ is empty
bool pq_empty(PriorityQueue *q) {
    if (q->top == 0) {
        return true;
    }
    return false;
}
// Checks if PQ is full
bool pq_full(PriorityQueue *q) {
    if (q->top == q->n + 1) {
        return true;
    }
    return false;
}
// Returns size of PQ
uint32_t pq_size(PriorityQueue *q) {
    return q->top;
}
// Enueues a node into PQ by assigning it potition top
// and incrementing top
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->arr[q->top] = n;
    q->top += 1;
    // Sorts PQ to adjust for newest node
    insertion_sort(q);
    return true;
}
// Reverse of enqueue
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    q->top -= 1;
    *n = q->arr[q->top];
    // No need to sort as if you take off top
    // the PQ will still satisfy order
    return true;
}
// Debug function to visualize PQ
void pq_print(PriorityQueue *q) {
    for (int i = 0; i < q->top; ++i) {
        printf("Node in position %d\n", i);
        node_print((q->arr[i]));
        puts("\n");
    }
}
