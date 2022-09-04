#include "code.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "io.h"

uint64_t bytes_read;
uint64_t bytes_written;

// static varibles avaliable to entire file for write_code()
static int write_bit_pointer = 0;
static uint8_t write_buffer[BLOCK];

// read_bytes is wrapper function for read()
// guarantess all requested bytes are read and returns
// amount read
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    ssize_t gamer;
    while (1) {
        gamer = read(infile, buf, nbytes);
        bytes_read += gamer;
        if (gamer < nbytes) {
            break;
        }
        if (gamer == nbytes) {
            break;
        }
    }
    return bytes_read;
}
// Wrapper function to write() nbytes from buffer
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    ssize_t gamer;
    size_t n = sizeof(*buf) / sizeof(buf[0]);
    while (1) {
        gamer = write(outfile, buf, nbytes);
        bytes_written += gamer;
        if (n - gamer == 0) {
            break;
        }
        if (gamer < nbytes) {
            break;
        }
        if (gamer == nbytes) {
            break;
        }
    }
    return bytes_written;
}
// Reads singular bit from buffer and returns
// through pointer
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t read_buffer[BLOCK];
    static int read_bit_pointer = 0;
    // Once static pointers hits end resets back to 0
    if (read_bit_pointer == BLOCK * 8) {
        read_bit_pointer = 0;
    }
    // Of static pointer is 0 then reads in new BLOCK of bytes into
    // static buffer
    if (read_bit_pointer == 0) {
        bytes_read += read(infile, read_buffer, BLOCK);
    }
    // Gets the exact position of bit
    uint32_t byte = read_bit_pointer / 8;
    uint32_t bits = read_bit_pointer - byte * 8;
    // Returns through bit
    *bit = ((read_buffer[byte] >> bits) & 1U); // Citation 1

    // If the bytes read is less than BLOCK returns false
    // to indicate there is more bytes in file
    read_bit_pointer += 1;
    if (bytes_read < BLOCK) {
        return false;
    }
    // Else EOF returns true
    return true;
}
// Writes Code c to outfile
void write_code(int outfile, Code *c) {
    uint32_t byte = write_bit_pointer / 8;
    uint32_t bits = write_bit_pointer - byte * 8;
    static int n = 0;
    // writes loops size of code times till all bits
    // inside code have been written
    while (n < c->top) {
        // If 1 write 1 into buffer else write 0
        if (code_get_bit(c, bits)) {
            write_buffer[byte] |= 1UL << bits;
        } else {
            write_buffer[byte] &= ~(1UL << bits);
        }
        n += 1;
    }
    // Resets static n to signify code was written
    // and new code wont start where last node left off
    if (n == c->top) {
        n = 0;
    }
    // Increments pointer by about of bits was in code
    write_bit_pointer += c->top;
    // If pointer his maximum then write buffer into outfile
    if (write_bit_pointer == BLOCK * 8) {
        write_bytes(outfile, write_buffer, BLOCK);
    }
}
// Writes out any codes that didnt perfectly fill buffer
void flush_codes(int outfile) {

    uint32_t byte = write_bit_pointer / 8;
    uint32_t bits = write_bit_pointer - byte * 8;
    // Assigns next byte worth of data after last bit written to 0
    // so that only important data is written and not junk
    for (int i = write_bit_pointer; i < write_bit_pointer + 8; ++i) {
        write_buffer[byte] &= ~(1UL << bits);
    }
    size_t bit_to_byte
        = write_bit_pointer / 8
          + (write_bit_pointer % 8 != 0); // celing divison to make sure to write entire bit

    // Resets pointer and writes buffer
    write_bit_pointer = 0;
    write_bytes(outfile, write_buffer, bit_to_byte);
}
