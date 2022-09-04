#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include "numtheory.h"
#include "randstate.h"

void lcm(mpz_t lamda, mpz_t p, mpz_t q) {
    mpz_t pp, qq, gcdpq;
    mpz_inits(pp, qq, gcdpq, NULL);
    mpz_set(pp, p);
    mpz_set(qq, q); // (pp, qq) = (p, q)
    mpz_sub_ui(pp, pp, 1);
    mpz_sub_ui(qq, qq, 1); // (pp, qq) = (pp - 1, qq - 1)
    gcd(gcdpq, pp, qq);
    mpz_mul(pp, pp, qq);
    mpz_abs(pp, pp);
    mpz_cdiv_q(lamda, pp, gcdpq);
    mpz_clears(pp, qq, gcdpq, NULL);
}

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t pp, lam;
    mpz_inits(pp, lam, NULL);
    uint64_t lower = nbits / 4, upper = (3 * nbits) / 4;
    uint64_t ranum = (rand() % (upper - lower + 1)) + lower;
    uint64_t remain = nbits - ranum;
    make_prime(p, ranum, iters); // generates primes for p and q
    make_prime(q, remain, iters);
    mpz_mul(n, p, q); // n = p * q
    lcm(lam, p, q);
    while (1) {
        mpz_urandomb(e, state, nbits);
        gcd(pp, e, lam); // makes sure their gcd is 1
        if (is_prime(e, iters) && mpz_cmp_ui(pp, 1) == 0) {
            break;
        }
    }
    mpz_clears(pp, lam, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t lam;
    mpz_init(lam);
    lcm(lam, p, q); // gets the lcm using helper function
    mod_inverse(d, e, lam);
    mpz_clear(lam);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    uint8_t k = (mpz_sizeinbase(n, 2) - 1) / 8; // Calculated block size
    uint8_t *ptr = calloc(k, sizeof(uint8_t *)); // Dyncamically allocate
    size_t j = 29;
    ptr[0] = 0xFF;
    uint8_t *after = (uint8_t *) calloc(k, sizeof(uint8_t *));
    while (feof(infile) != true) {
        j = fread(&ptr[1], 1, k - 1, infile); // reads infile to fill up after ptr one
        mpz_import(
            m, j + 1, 1, sizeof(uint8_t), 0, 0, ptr); // imports only read data using j from fread
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    mpz_clears(m, c, NULL);
    free(ptr);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    uint8_t k = (mpz_sizeinbase(n, 2) - 1) / 8; // calcualted block size
    uint8_t *ptr = calloc(k, sizeof(uint8_t *)); // dynamically allocates
    unsigned long var = 1;
    unsigned long *t = &var;
    size_t j = 100;
    fseek(infile, 0, SEEK_SET); // resets file to top
    while (!feof(infile)) { // checks if reached end of file
        gmp_fscanf(infile, "%Zx\n", c); // reverse of rsa_encrypt_file
        rsa_decrypt(m, c, d, n);
        mpz_export(ptr, &j, 1, sizeof(uint8_t), 0, 0, m);
        fwrite(&ptr[1], sizeof(k), j - 1, outfile);
    }
    mpz_clears(m, c, NULL);
    free(ptr);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n); // checks if the given s matches m
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    } else {
        mpz_clear(t);
        return false;
    }
}
