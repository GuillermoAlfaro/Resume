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
    mpz_t n, d, e;
    mpz_inits(n, d, e, NULL);
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile = fopen("rsa.priv", "r");
    bool v, h;

    while ((opt = getopt(argc, argv, "-i:-o:-n:vh")) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pvfile = fopen(optarg, "r"); break;
        case 'v': v = true; break;
        case 'h': h = true; break;
        default: break;
        }
    }
    // If there was an error openeing the files then report to user and quit
    if (infile == NULL || outfile == NULL || pvfile == NULL) {
        puts("Failed to open file");
        return 0;
    }
    rsa_read_priv(n, d, pvfile); // n and d are filled
    if (v) {
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    if (h) {
        puts("SYNOPSIS");
        puts("   Decrypts data using RSA decryption.");
        puts("   Encrypted data is encrypted by the encrypt program.\n");
        puts("USAGE");
        puts("   ./decrypt [OPTIONS]\n");
        puts("OPTIONS");
        puts("   -h              Display program help and usage.");
        puts("   -v              Display verbose program output.");
        puts("   -i infile       Input file of data to decrypt (default: stdin).");
        puts("   -o outfile      Output file for decrypted data (default: stdout).");
        puts("   -n pvfile       Private key file (default: rsa.priv).");
        fclose(infile);
        fclose(outfile);
        mpz_clears(n, d, e, NULL);
        return 0;
    }
    // Decrypts data and writes to infile
    rsa_decrypt_file(infile, outfile, n, d);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, d, e, NULL);
    return 1;
}
