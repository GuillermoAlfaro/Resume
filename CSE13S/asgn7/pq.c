#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PriorityQueue PriorityQueue;

typedef struct Pairs Pairs;

struct Pairs {
    char *author;
    double distance;
};

struct PriorityQueue {
    uint32_t capacity;
    Pairs **arr;
    uint32_t top;
};

void insertion_sort(PriorityQueue *q) {
    for (uint32_t ii = 0; ii < q->top; ii++) {
        uint32_t jj = ii;
        Pairs *temp = q->arr[ii];
        while (jj > 0 && temp->distance > (q->arr[jj - 1])->distance) {
            q->arr[jj] = (q->arr[jj - 1]);
            jj -= 1;
        }
        q->arr[jj] = temp;
    }
}
Pairs *pairs_create(char *author, double *distance) {
    Pairs *d = (Pairs *) malloc(sizeof(Pairs));
    d->distance = *distance;
    d->author = author;
    return d;
}

void free_pairs(Pairs **p) {
    free((*p));
    *p = NULL;
}

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->arr = (Pairs **) malloc(capacity * sizeof(Pairs));
    pq->capacity = capacity;
    pq->top = 0;
    return pq;
}

void pq_delete(PriorityQueue **q) {
    free((*q)->arr);
    (*q)->arr = NULL;
    free(*q);
    *q = NULL;
}

bool pq_empty(PriorityQueue *q) {
    if (q->top == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    if (q->top == q->capacity + 1) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->capacity;
}

bool enqueue(PriorityQueue *q, char *author, double dist) {
    if (pq_full(q)) {
        return false;
    }
    char copy[strlen(author)];
    strcpy(copy, author);
    Pairs *n = pairs_create(author, &dist);
    q->arr[q->top] = n;
    q->top += 1;
    insertion_sort(q);
    return true;
}

bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (pq_empty(q)) {
        return false;
    }
    q->top -= 1;
    Pairs *w = q->arr[q->top];
    *author = w->author;
    *dist = w->distance;
    free_pairs(&q->arr[(q->top)]);
    return true;
}

void pairs_print(Pairs *p) {
    printf("%s %f\n", p->author, p->distance);
}

void pq_print(PriorityQueue *q) {
    for (int i = 0; i < q->top; ++i) {
        printf("Pair in position %d\n", i);
        pairs_print((q->arr[i]));
        puts("\n");
    }
}
