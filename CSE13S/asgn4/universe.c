

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "universe.h"
#include <inttypes.h>
#include <ncurses.h>
#include <unistd.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool torodial;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *a = (Universe *) malloc(sizeof(Universe));
    a->grid = (_Bool **) calloc(rows, sizeof(_Bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        a->grid[r] = (_Bool *) calloc(cols, sizeof(_Bool));
    }
    a->torodial = toroidal;
    a->rows = rows;
    a->cols = cols;
    return a;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < uv_rows(u); i++) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    // Iterates till reaching EOF then breaks infinite loop
    while (true) {
        if (feof(infile)) {
            break;
        }
        uint32_t real, ceal;
        fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &real, &ceal);
        uv_live_cell(u, real, ceal);
    }
    return true;
}

// Eugene Section example 1/28/22 for getting previous number if torodial
uint32_t uv_prev(Universe *u, uint32_t x, bool roc) {
    uint32_t doing;
    if (roc == true) {
        doing = uv_rows(u);
    } else {
        doing = uv_cols(u);
    }
    if (x >= 0 && x < doing) {
        int index = (x + doing - 1) % doing;
        return index;
    }
    return -1;
}
// Eugene Section example 1/28/22 for getting next number if torodial
uint32_t uv_next(Universe *u, uint32_t x, bool roc) {
    uint32_t doing;
    if (roc == true) {
        doing = uv_rows(u);
    } else {
        doing = uv_cols(u);
    }
    if (x >= 0 && x < doing) {
        int index = (x + 1) % doing;
        return index;
    }
    return -1;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    // Converts uint32 to int to be used later
    int rr = r, cc = c;
    uint32_t count = 0;
    uint32_t r_temp;
    uint32_t c_temp;
    bool edge_case = false;
    if (r == 0 || r + 1 == uv_rows(u)) {
        edge_case = true;
    }
    if (c == 0 || c + 1 == uv_cols(u)) {
        edge_case = true;
    }
    // If cell is NOT at edge enter this if()
    // or if universe is not torodial
    if (u->torodial == false || edge_case == false) {
        for (int j = rr - 1; j < rr + 2; j++) {
            for (int x = cc - 1; x < cc + 2; x++) {
                // To make sure grid[j][x] doesn't try to look at numbers outside of array
                if (j != uv_rows(u) && x != uv_cols(u) && j != -1 && x != -1) {
                    if (u->grid[j][x] == true) {
                        count += 1;
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < 3; ++i) {
            if (i == 0) {
                r_temp = uv_prev(u, r, true);
            }
            if (i == 1) {
                r_temp = r;
            }
            if (i == 2) {
                r_temp = uv_next(u, r, true);
            }
            for (int j = 0; j < 3; ++j) {
                if (j == 0) {
                    c_temp = uv_prev(u, c, false);
                }
                if (j == 1) {
                    c_temp = c;
                }
                if (j == 2) {
                    c_temp = uv_next(u, c, false);
                }
                if (u->grid[r_temp][c_temp] == true) {
                    count += 1;
                }
            }
        }
    }
    // Since the for() loops go over the cell in question, we subtract a count if the cell is alive
    if (uv_get_cell(u, r, c) == true) {
        count -= 1;
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    bool temp;
    for (uint32_t r = 0; r < uv_rows(u); r++) {
        for (uint32_t c = 0; c < uv_cols(u); c++) {
            temp = u->grid[r][c];
            if (temp == true) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
