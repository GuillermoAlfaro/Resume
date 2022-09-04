#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "math.h"
#include "quick.h"
#include "set.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum { h, b, i, q } pokemon;
int main(int argc, char **argv) {
    int opt = 0;
    double r = 13371453;
    double n = 100;
    double p = 100;
    bool H = false;
    Set s = empty_set();

    while ((opt = getopt(argc, argv, "ahbiqr:n:p:H")) != -1) {
        switch (opt) {
        case 'a': s = complement_set(empty_set()); break;
        case 'h': s = insert_set(0, s); break;
        case 'b': s = insert_set(1, s); break;
        case 'i': s = insert_set(2, s); break;
        case 'q': s = insert_set(3, s); break;
        case 'r': r = strtod(optarg, NULL); break;
        case 'n': n = strtod(optarg, NULL); break;
        case 'p': p = strtod(optarg, NULL); break;
        case 'H': H = true; break;
        default: H = true; break;
        }
    }
    uint32_t *A = (uint32_t *) malloc(n * sizeof(uint32_t));
    if (n < p) {
        p = n;
    }
    p = floor(p / 5);
    Stats data;
    //CODE CITATION BLOCK 1//
    if (H) {
        int s;
        FILE *file;
        file = fopen("help.txt", "r");
        while ((s = fgetc(file)) != EOF) {
            printf("%C", s);
        }
        fclose(file);
        return 0;
    }
    //CODE CITATION BLOCK 1//
    for (pokemon o = h; o <= q; o += 1) {
        if (member_set(o, s)) {
            switch (o) {
            case 0:
                srand(r);
                reset(&data);
                for (uint32_t i = 0; i < n; i++) {
                    A[i] = rand() & 0x3FFFFFFF;
                }
                heap_sort(&data, A, n);
                printf("\nHeap Sort, %.0f elements, %" PRIu64 " moves, %" PRIu64 " compares", n,
                    data.moves, data.compares);
                for (uint32_t j = 0; j < p; j += 1) {
                    puts("\n");
                    for (int i = 0; i < 5; i++) {
                        printf("%13" PRIu32, *(A + i + j * 5));
                    }
                }
                break;
            case 1:
                srand(r);
                reset(&data);
                for (uint32_t i = 0; i < n; i++) {
                    A[i] = rand() & 0x3FFFFFFF;
                }
                batcher_sort(&data, A, n);
                printf("\nBatcher Sort, %.0f elements, %" PRIu64 " moves, %" PRIu64 " compares", n,
                    data.moves, data.compares);
                for (uint32_t j = 0; j < p; j += 1) {
                    puts("\n");
                    for (int i = 0; i < 5; i++) {
                        printf("%13" PRIu32, *(A + i + j * 5));
                    }
                }

                break;
            case 2:
                srand(r);
                reset(&data);
                for (uint32_t i = 0; i < n; i++) {
                    A[i] = rand() & 0x3FFFFFFF;
                }
                insertion_sort(&data, A, n);
                printf("\nInsertion Sort, %.0f elements, %" PRIu64 " moves, %" PRIu64 " compares",
                    n, data.moves, data.compares);
                for (uint32_t j = 0; j < p; j += 1) {
                    puts("\n");
                    for (int i = 0; i < 5; i++) {
                        printf("%13" PRIu32, *(A + i + j * 5));
                    }
                }

                break;
            case 3:
                srand(r);
                reset(&data);
                for (uint32_t i = 0; i < n; i++) {
                    A[i] = rand() & 0x3FFFFFFF;
                }
                quick_sort(&data, A, n);
                printf("\nQuick Sort, %.0f elements, %" PRIu64 " moves, %" PRIu64 " compares", n,
                    data.moves, data.compares);
                for (uint32_t j = 0; j < p; j += 1) {
                    puts("\n");
                    for (int i = 0; i < 5; i++) {
                        printf("%13" PRIu32, *(A + i + j * 5));
                    }
                }

                break;
            }
        }
    }
    puts("\n");
    free(A);
    return 1;
}
