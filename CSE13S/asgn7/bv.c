#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct BitVector {
    uint32_t length;
    uint8_t *vector;
} BitVector;

BitVector *bv_create(uint32_t length) {
    BitVector *var = (BitVector *) malloc(sizeof(BitVector)); // Creates space for the BitVector
    var->vector
        = (uint8_t *) malloc(sizeof(uint8_t) * length); // Creates space for the array of uint8_t's
    var->length = length;
    // Initializes the array to 0
    for (int i = 0; i < length; ++i) {
        var->vector[i] = 0;
    }
    return var;
}
// Frees all allocated space and sets them to NULL
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    (*bv)->vector = NULL;
    free((*bv));
    (*bv) = NULL;
}
// Returns length
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}
// Sets the exact bit at i to 1
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > 8 * bv_length(bv) || i < 0) {
        return false;
    }
    bv->vector[i / 8]
        |= (1 << (i % 8)); // Code on how to set a bit using bitwise manipulation from Eugene
    return true;
}
// Clears the exact bit at i
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > 8 * bv_length(bv) || i < 0) {
        return false;
    }
    bv->vector[i / 8]
        &= ~(1 << (i % 8)); // Code on how to clear a bit using bitwise manipulation from Eugene
    return true;
}
// Returns the bool at i
bool bv_get_bit(BitVector *bv, uint32_t i) {
    uint32_t byte = i / 8;
    uint32_t bit = i - byte * 8;
    return ((bv->vector[byte] >> bit) & 1U); // Citation 1
}
// Debug to see what im doing in side the matrix
void bv_print(BitVector *bv) {
    for (int i = 0; i < bv_length(bv) * 8; ++i) {
        if (bv_get_bit(bv, i)) {
            printf("position %d is true\n", i);
        }
    }
}
