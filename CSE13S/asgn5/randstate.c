#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <inttypes.h>
#include <stdlib.h>

gmp_randstate_t state; // global variable: state
    // to be used across multiple files

void randstate_init(uint64_t seed) {
    srandom(seed); // seeds randoms and initizliaed state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

void randstate_clear(void) {
    gmp_randclear(state); // free state
}
