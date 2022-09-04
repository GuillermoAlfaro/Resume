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

int main(int argc, char **argv) {
    int opt = 0, infile = 0, outfile = 1, tree_counter = 0;
    bool v, h;
    uint64_t hist[ALPHABET] = { 0 };
    uint8_t buffer[BLOCK];

    // Creates the header stuct and fills in
    // all the data
    Header head;
    struct stat sb;
    head.magic = MAGIC;
    head.file_size = sb.st_size;
    head.permissions = sb.st_mode;

    while ((opt = getopt(argc, argv, "-i:-o:vh")) != -1) {
        switch (opt) {
        case 'i':
            infile = open(optarg, O_APPEND);
            fstat(infile, &sb);
            break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, sb.st_mode); break;
        case 'v': v = true; break;
        case 'h': h = true; break;
        default: break;
        }
    }
    // First thing written is header
    write(outfile, &head, sizeof(head));
    if (h) {
        puts("HELP SCREEN");
        return 0;
    }
    if (v) {
        puts("VERBOSE");
    }

    // These variabls are to track bytes read and distingush how many are left to read
    uint64_t difference;
    uint64_t previous_total = 0;
    uint64_t cummulative_running_total;

    printf("bytes in file: %lld\n", sb.st_size);

    // This while loop creates histagram
    while (1) {

        uint64_t running_total = read_bytes(infile, buffer, BLOCK);
        difference = running_total - previous_total;
        // Iterates up untill difference to only read new data written to buffer
        for (int i = 0; i < difference; ++i) {
            hist[buffer[i]] += 1;
        }
        previous_total = running_total;
        cummulative_running_total = running_total;
        // if we write out all data from infile then break
        if (running_total == sb.st_size) {
            break;
        }
    }
    printf("Total bytes read %llu\n", previous_total);

    Node *root;

    root = build_tree(hist);
    Code table[ALPHABET] = { 0 };
    build_codes(root, table);
    dump_tree(outfile, root);

    // resets infile to top of file to
    // reread all of the bytes and output their respective code
    // in this while loop
    lseek(infile, 0, SEEK_SET);
    uint64_t new_different_total = 0;
    while (1) {
        uint64_t running_total = read_bytes(infile, buffer, BLOCK);
        difference = running_total - cummulative_running_total;
        for (int i = 0; i < difference; ++i) {
            write_code(outfile, &table[buffer[i]]);
        }
        new_different_total += difference;
        cummulative_running_total = running_total;
        if (new_different_total == sb.st_size) {
            break;
        }
    }

    // flush any data the could have been left behind
    // closes files
    flush_codes(outfile);
    close(outfile);
    close(infile);
}
