#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

int main(int argc, char **argv) {
    mpz_t n, e, signiture, message, newsig;
    mpz_inits(n, e, signiture, message, newsig, NULL);
    int opt = 0;
    char username[256];
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile = fopen("rsa.pub", "r");
    bool v, h;

    while ((opt = getopt(argc, argv, "-i:-o:-n:vh")) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pbfile = fopen(optarg, "r"); break;
        case 'v': v = true; break;
        case 'h': h = true; break;
        default: break;
        }
    }
    if (infile == NULL || outfile == NULL || pbfile == NULL) {
        puts("Failed to open file");
        return 0;
    }
    rsa_read_pub(n, e, signiture, username, pbfile);
    if (v) {
        printf("user = %s\n", username);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(signiture, 2), signiture);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    if (h) {
        puts("SYNOPSIS");
        puts("   Encrypts data using RSA encryption.");
        puts("   Encrypted data is decrypted by the decrypt program.\n");
        puts("USAGE");
        puts("   ./encrypt [OPTIONS]\n");
        puts("OPTIONS");
        puts("   -h              Display program help and usage.");
        puts("   -v              Display verbose program output.");
        puts("   -i infile       Input file of data to encrypt (default: stdin).");
        puts("   -o outfile      Output file for encrypted data (default: stdout).");
        puts("   -n pbfile       Public key file (default: rsa.pub).");
        fclose(infile);
        fclose(outfile);
        fclose(pbfile);
        mpz_clears(n, e, signiture, message, newsig, NULL);
        return 0;
    }
    mpz_set_str(message, username, 62);
    if (!rsa_verify(message, signiture, e, n)) {
        puts("signiture was NOT verified");
        mpz_clears(n, e, signiture, message, newsig, NULL);
        return 0;
    }
    rsa_encrypt_file(infile, outfile, n, e);
    fclose(pbfile);
    mpz_clears(n, e, signiture, message, newsig, NULL);
    return 1;
}
