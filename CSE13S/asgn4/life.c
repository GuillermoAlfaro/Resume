
#include <stdio.h>
#include <stdlib.h>
#include "universe.h"
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#define DELAY 50000
int main(int argc, char **argv) {
    int opt = 0;
    double n = 100;
    uint32_t rops, cops;
    bool t = false, s = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    while ((opt = getopt(argc, argv, "tsn:-i:-o:")) != -1) {
        switch (opt) {
        case 't': t = true; break;
        case 's': s = true; break;
        case 'n': n = strtod(optarg, NULL); break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        default: break;
        }
    }
    if (n < 0) {
        fprintf(outfile, "Can't have less than 0 iterations\n");
        return 0;
    }
    fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &rops, &cops);
    if (rops < 1 || cops < 1) {
        fprintf(outfile, "Invalid rows and colums\n");
        return 0;
    }
    Universe *A = uv_create(rops, cops, t);
    Universe *B = uv_create(rops, cops, t);
    Universe *swap;
    if (!uv_populate(A, infile)) {
        fprintf(outfile, "Cell out of bounds\n");
        return 0;
    }
    fclose(infile);
    // Starts screen if (s) is not true
    if (!s) {
        initscr();
        curs_set(false);
    }

    for (int i = 0; i < n; ++i) {
        refresh();
        usleep(DELAY);
        clear();
        for (int j = 0; j < uv_rows(A); ++j) {
            for (int k = 0; k < uv_cols(A); ++k) {
                uint32_t cenSUS = uv_census(A, j, k);
                // If the cell has 2 or 3 neighbors and was alive, it stays alive
                if (2 <= cenSUS && cenSUS <= 3 && uv_get_cell(A, j, k)) {
                    uv_live_cell(B, j, k);
                    // If the cell was dead and had 3 alive neighbors it becomes alive
                } else if (cenSUS == 3) {
                    uv_live_cell(B, j, k);
                    // Else it is dead
                } else {
                    uv_dead_cell(B, j, k);
                }
                // Prints to screen if (s) is not true
                if (uv_get_cell(B, j, k) && !s) {
                    mvprintw(j, k, "o");
                } else if (!s) {
                    mvprintw(j, k, " ");
                }
            }
        }
        // Swaps the universes around
        swap = A;
        A = B;
        B = swap;
    }
    if (!s) {
        endwin();
    }
    // Prints out final state of universe after loop
    uv_print(A, outfile);
    fclose(outfile);
    uv_delete(A);
    uv_delete(B);

    return 1;
}
