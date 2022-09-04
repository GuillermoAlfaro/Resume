#include "bv.h"
#include "speck.h"
#include "salts.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
} BloomFilter;

// Creates space for BF and initializes a BV to assign in BF.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf == NULL) {
        return NULL;
    }
    bf->filter = bv_create(size);
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    return bf;
}
// Frees the BV then BF.
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
}
// Returns size of BF which is size of BV times amount of bits.
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}
// Hashes the word using each salt and sets their output to 1 in the BV of the BF.
void bf_insert(BloomFilter *bf, char *word) {
    bv_set_bit(bf->filter, hash(bf->primary, word) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, word) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, word) % bf_size(bf));
}
//  Hashes the word and checks if the bit at each location is true, if all are then the word exists in BV.
bool bf_probe(BloomFilter *bf, char *word) {
    bool first = bv_get_bit(bf->filter, hash(bf->primary, word) % bf_size(bf));
    bool second = bv_get_bit(bf->filter, hash(bf->secondary, word) % bf_size(bf));
    bool third = bv_get_bit(bf->filter, hash(bf->tertiary, word) % bf_size(bf));
    if (first && second && third) {
        return true;
    } else {
        return false;
    }
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
