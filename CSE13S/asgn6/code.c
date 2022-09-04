#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t top;
    uint8_t bits[MAX_CODE_SIZE];
} Code;

Code code_init(void) {
    Code var;
    var.top = 0;
    int interate = 0;
    // Sets all the bytes equal to 0
    while (interate < MAX_CODE_SIZE) {
        var.bits[interate] = 0;
        ++interate;
    }
    return var;
}
// Returns top of code as code_size
uint32_t code_size(Code *c) {
    return c->top;
}
// If the top is 0 then the code is empty
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}
// If the top has ALPHABET number of bytes than it is full
bool code_full(Code *c) {
    if (c->top == ALPHABET) {
        return true;
    }
    return false;
}
// Sets the exact bit at i to 1
bool code_set_bit(Code *c, uint32_t i) {
    if (i > ALPHABET || i < 0) {
        return false;
    }
    // Floor divides i by 8 to know how many bytes to move into
    uint32_t byte = i / 8;
    // Assigns remainder to bit
    uint32_t bit = i - byte * 8;
    // Uses bitwise OR to move to the bit and change it to 1 if not already
    c->bits[byte] |= 1UL << bit; // Citation 1
    return true;
}
// sets the exact bit at i to 0
bool code_clr_bit(Code *c, uint32_t i) {
    if (i > ALPHABET || i < 0) {
        return false;
    }
    // Same arithmetic as code_set_bit()
    uint32_t byte = i / 8;
    uint32_t bit = i - byte * 8;
    // Uses bitwise AND and NOT to change the bit to 0 is not already
    c->bits[byte] &= ~(1UL << bit); // Citation 1
    return true;
}
// returns the bit as bool at i
bool code_get_bit(Code *c, uint32_t i) {
    uint32_t byte = i / 8;
    uint32_t bit = i - byte * 8;
    return ((c->bits[byte] >> bit) & 1U); // Citation 1
}
// Sets the bit 1 after top to either 1 or 0
bool code_push_bit(Code *c, uint8_t bit) {
    // Checks if Code c is full before trying to push
    if (code_full(c)) {
        return false;
    }
    // if 1 sets bit to 1 and increments top
    if (bit == 1) {
        code_set_bit(c, c->top);
        c->top += 1;
        return true;
    }
    // if 0 clears bit to 0 and increments top
    if (bit == 0) {
        code_clr_bit(c, c->top);
        c->top += 1;
        return true;
    }
    return false;
}
// Assigns the poitner bit to the bit currently at top
// then decrements the top
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }

    *bit = code_get_bit(c, c->top - 1);
    c->top -= 1;
    return true;
}
// debug function to visualize Codes
void code_print(Code *c) {
    for (int i = 0; i < c->top; i++) {
        printf("%d", code_get_bit(c, i));
    }
}
