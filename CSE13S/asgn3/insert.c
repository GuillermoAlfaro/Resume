#include "insert.h"

#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    Stats(reset);
    uint32_t i = 1;
    for (i; i < n; i++) {
        uint32_t j = i;
        uint32_t temp = move(stats, *(A + i));
        while (j > 0 && temp < *(A + j - 1)) {
            cmp(stats, j, 0);
            *(A + j) = move(stats, *(A + j - 1));
            j -= 1;
        }
        *(A + j) = move(stats, temp);
    }
}
