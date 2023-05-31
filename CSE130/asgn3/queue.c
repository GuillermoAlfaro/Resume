#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>

// Structs ---------------------------------------------------------------------------------------------
typedef struct NodeObj *node;
typedef struct NodeObj {
    void *elem;
    node prev;
    node next;
} NodeObj;

struct queue {
    pthread_cond_t nEmpty;
    pthread_cond_t nFull;
    pthread_mutex_t lock;

    uint64_t size;
    uint64_t len;

    node top;
    node bottom;
};

// Constructors-Desctructors ---------------------------------------------------------------------------

node newNode(void *elem) {
    node n = malloc(sizeof(NodeObj));
    n->elem = elem;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void freeNode(node *n) {
    if (n != NULL && *n != NULL) {
        free(*n);
        *n = NULL;
        n = NULL;
    }
}

queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    q->size = size;
    q->len = 0;
    q->top = q->bottom = NULL;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->nEmpty, NULL);
    pthread_cond_init(&q->nFull, NULL);
    return q;
}

void queue_delete(queue_t **q) {
    if ((*q) != NULL && q != NULL) {
        printf("q->len: %ld\n", (*q)->len);
        for (uint64_t i = 0; i < (*q)->len; i++) {
            printf("this\n");
            void *elem = NULL;
            queue_pop((*q), (&elem));
        }

        pthread_mutex_destroy(&(*q)->lock);
        pthread_cond_destroy(&(*q)->nEmpty);
        pthread_cond_destroy(&(*q)->nFull);
        free(*q);
        *q = NULL;
    }
}

// Acess functions --------------------------------------------------------------------------------------

// Should block is q is full
// Should return true, unless given invalid pointer (NULL)
bool queue_push(queue_t *q, void *elem) {
    pthread_mutex_lock(&q->lock);
    while (q->len == q->size) {
        pthread_cond_wait(&q->nFull, &q->lock);
    }
    node n = newNode(elem);
    if (q->len == 0 && q->size != 0) {
        q->top = n;
        q->bottom = n;
        q->len += 1;
    } else if (q->len < q->size) {
        q->bottom->prev = n;
        n->next = q->bottom;
        q->bottom = n;
        q->len += 1;
    }
    pthread_cond_broadcast(&q->nEmpty);
    pthread_mutex_unlock(&q->lock);

    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    pthread_mutex_lock(&q->lock);
    while (q->len == 0 && q->size != 0) {
        pthread_cond_wait(&q->nEmpty, &q->lock);
    }

    if (q == NULL) {
        return false;
    }
    if (q->len == 1) {
        (*elem) = q->top->elem;
        freeNode(&q->top);
        q->top = NULL;
        q->bottom = NULL;
        q->len -= 1;
    } else if (q->len > 1) {
        (*elem) = q->top->elem;
        node temp = q->top->prev;
        temp->next = NULL;
        freeNode(&q->top);
        q->top = temp;
        q->len -= 1;
    }
    pthread_cond_broadcast(&q->nFull);
    pthread_mutex_unlock(&q->lock);
    return true;
}
