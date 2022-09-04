#include "heap.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left, right;
    left = 2 * first;
    right = left + 1;
    if ((right <= last) & (*(A + right - 1) > *(A + left - 1))) {
        int f = cmp(stats, *(A + right - 1), *(A + left - 1));
        int g = cmp(stats, right, left);
        cmp(stats, f, g);
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);
    uint32_t temp = 0;
    uint32_t temp2 = floor(last / 2);
    while ((mother <= temp2) & !found) {
        if (*(A + great - 1) > *(A + mother - 1)) {
            cmp(stats, *(A + great - 1), *(A + mother - 1));
            swap(stats, (A + mother - 1), (A + great - 1));
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t temp = floor(last / 2);
    for (uint32_t father = temp; temp > (first - 1); temp--) {
        father++;
        fix_heap(stats, A, temp, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    uint32_t first, last, temp;
    first = 1;
    last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; last > first; last--) {
        leaf++;
        swap(stats, (A + first - 1), (A + last - 1));
        fix_heap(stats, A, first, last - 1);
    }
}
