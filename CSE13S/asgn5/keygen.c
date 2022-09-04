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
    mpz_t p, q, n, d, e, signiture, message, cipher;
    mpz_inits(p, q, n, d, e, signiture, message, cipher, NULL);
    char *username;
    int opt = 0;
    uint64_t b = 256, i = 50;
    uint64_t s = 0;
    bool v = false, h = false;
    FILE *pbfile = fopen("rsa.pub", "w+"); // opens in writing to clear file
    FILE *pvfile = fopen("rsa.priv", "w+");
    while ((opt = getopt(argc, argv, "b:i:-n:-d:s:vh")) != -1) {
        switch (opt) {
        case 'b': b = strtod(optarg, NULL); break;
        case 'i': i = strtod(optarg, NULL); break;
        case 'n': pbfile = fopen(optarg, "w+"); break;
        case 'd': pvfile = fopen(optarg, "w+"); break;
        case 's': s = strtod(optarg, NULL); break;
        case 'v': v = true; break;
        case 'h': h = true; break;
        default: break;
        }
    }
    if (b < 0) {
        fprintf(stdout, "Can't have less than 0 bits for public modulus n\n");
        mpz_clears(p, q, n, d, e, signiture, message, cipher, NULL);
        return 0;
    }
    if (h) {
        puts("SYNOPSIS");
        puts("   Generates an RSA public/private key pair.");
        puts("\nUSAGE");
        puts("   ./keygen [OPTIONS]");
        puts("\nOPTIONS");
        puts("   -h              Display program help and usage.");
        puts("   -v              Display verbose program output.");
        puts("   -b bits         Minimum bits needed for public key n (default: 256).");
        puts("   -i iterations   Miller-Rabin iterations for testing primes (default: 50).");
        puts("   -n pbfile       Public key file (default: rsa.pub).");
        puts("   -n pbfile       Public key file (default: rsa.pub).");
        puts("   -d pvfile       Private key file (default: rsa.priv).");
        puts("   -s seed         Random seed for testing.");
        mpz_clears(p, q, n, d, e, signiture, message, cipher, NULL);
        return 0;
    }
    // checks if s is 0, since 0 is initilized to zero it sees if it was changed
    // if not then used time(NULL) as randstate_init see
    if (s == 0) {
        randstate_init(time(NULL));
    } else {
        randstate_init(s);
    }
    // Checks permissions of pvfile, if it is set to rw only for user then do nothing, else set it to that
    if (fileno(pvfile) != 600) {
        int fd = open("rsa.priv", O_RDONLY);
        fchmod(fd, S_IRUSR | S_IWUSR);
        close(fd);
    }
    // Generates public and private keys
    rsa_make_pub(p, q, n, e, b, i);
    rsa_make_priv(d, e, p, q);
    // Gets the username to be usee for signiture
    username = getenv("USER");
    mpz_set_str(message, username, 62);
    rsa_sign(signiture, message, d, n);
    // Writes public and private data to respective files
    rsa_write_pub(n, e, signiture, username, pbfile);
    rsa_write_priv(n, d, pvfile);
    if (v) {
        printf("user = %s\n", username);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(signiture, 2), signiture);
        gmp_printf("p (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, d, e, signiture, message, cipher, NULL);
    return 1;
}
