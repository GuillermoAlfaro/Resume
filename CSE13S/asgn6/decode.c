#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include "code.h"
#include "huffman.h"
#include "header.h"
#include "defines.h"

int main(int argc, char **argv) {
    int opt = 0, infile = 0, outfile = 1;
    bool v, h;

    while ((opt = getopt(argc, argv, "-i:-o:vh")) != -1) {
        switch (opt) {
        case 'i': infile = open(optarg, O_APPEND); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        case 'v': v = true; break;
        case 'h': h = true; break;
        default: break;
        }
    }

    uint8_t buffer[16] = { 0 };
    read(infile, buffer, 1);

    for (int i = 0; i < 1; ++i) {
        printf("%u\n", buffer[i]);
    }
}
