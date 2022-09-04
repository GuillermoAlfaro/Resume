#include "batcher.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (*(A + x) > *(A + y)) {
        swap(stats, (A + x), (A + y));
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    if (n == 0) {
        exit(1);
    }
    uint32_t i;
    uint32_t t = log2(n) + 1;
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (i = 0; i < n - d; i++) {
                if ((i & p) == r) {
                    cmp(stats, (i & p), r);
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
