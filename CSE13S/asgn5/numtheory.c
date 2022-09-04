#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t, aa, bb;
    mpz_inits(t, aa, bb, NULL); // int t;
    mpz_set(aa, a); // copies a so its not changed
    mpz_set(bb, b); // copies b so its not changed
    while (mpz_cmp_ui(bb, 0)) {
        mpz_set(t, bb); // t = b;
        mpz_mod(bb, aa, bb); // b = a % b;
        mpz_set(aa, t); // a = t;
    }
    mpz_set(d, aa); // *d = a;
    mpz_clears(t, aa, bb, NULL);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, rr, t, tt, q, qhold, rhold, thold;
    mpz_inits(r, rr, t, tt, q, qhold, rhold, thold, NULL);
    mpz_set(r, n);
    mpz_set(rr, a); // (r, r') = (n, a)
    mpz_set_d(t, 0);
    mpz_set_d(tt, 1); // (t, t') = (0, 1)
    while (mpz_cmp_ui(rr, 0) != 0) {
        mpz_fdiv_q(q, r, rr);
        mpz_set(qhold, q);

        mpz_mul(q, q, rr); // q = q * r'
        mpz_set(rhold, r); // rhold = r
        mpz_set(r, rr); // r = r'
        mpz_sub(rr, rhold, q); // r' = rhold - q

        mpz_mul(qhold, qhold, tt); // qhold = qhold * t'
        mpz_set(thold, t); // thold = t
        mpz_set(t, tt); // t = t'
        mpz_sub(tt, thold, qhold); // t' = thold - qhold
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0); // return i = 0
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n); // t = t + n
    }
    if (mpz_cmp_ui(r, 1) != 1) {
        mpz_set(i, t); // return i = t
    }
    mpz_clears(r, rr, t, tt, q, qhold, rhold, thold, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, two, tbase, texpo, tmod;
    mpz_init_set(tbase, base);
    mpz_init_set(texpo, exponent);
    mpz_init_set(tmod, modulus);
    mpz_init_set_ui(two, 2); // two = 2;
    mpz_init_set_ui(v, 1); // v = 1
    mpz_init_set(p, base); // p = base;

    while (mpz_cmp_ui(exponent, 0)) { // exponent > 0
        if (mpz_odd_p(exponent)) { // if odd
            mpz_mul(v, v, p); // v = (v * p) % modulus;
            mpz_mod(v, v, modulus);
        }
        mpz_mul(p, p, p); // p = (p * p) % modulus;
        mpz_mod(p, p, modulus);
        mpz_fdiv_q(exponent, exponent, two); // exponent = exponent / 2;
    }
    mpz_set(base, tbase);
    mpz_set(exponent, texpo);
    mpz_set(modulus, tmod);
    mpz_set(out, v); // *out = v;
    mpz_clears(v, p, two, tbase, texpo, tmod, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    if (mpz_cmp_ui(n, 2) < 0) {
        return false;
    } // if (n < 2) return false
    mpz_t temp, nn, r, s, temp2, temp3, two, a, y, nnn;
    mpz_inits(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
    mpz_sub_ui(nnn, n, 3); // int nnn = n - 3
    mpz_sub_ui(nn, n, 1); // int nn = n - 1
    mpz_sub_ui(r, n, 1); // int r = n - 1
    mpz_set_d(s, 1); // int s = 1
    mpz_set_d(two, 2);
    mpz_mod(temp, n, two);
    // if (n != 2 && n % 2 == 0) return false
    if (mpz_cmp_ui(n, 2) != 0 && mpz_cmp_ui(temp, 0) == 0) {
        mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
        return false;
    } // if (n == 2) return true
    if (mpz_cmp_ui(n, 2) == 0) {
        mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
        return true;
    }
    // if (n == 3) return true
    // n being three caused errors when generating a random int so
    // I gave it its own excpetion
    if (mpz_cmp_ui(n, 3) == 0) {
        mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
        return true;
    }
    mpz_mod_ui(temp2, r, 2);
    while (mpz_cmp_ui(temp2, 0) == 0) { // this while loop generates r
        mpz_cdiv_q_ui(r, r, 2);
        mpz_mod_ui(temp2, r, 2);
    }
    int ss = 1;
    mpz_pow_ui(temp3, two, ss);
    mpz_mul(temp3, temp3, r);
    while (mpz_cmp(temp3, nn) != 0) {
        mpz_add_ui(s, s, 1);
        ss += 1;
        mpz_pow_ui(temp3, two, ss);
        mpz_mul(temp3, temp3, r);
    }
    for (int i = 1; i < iters; ++i) {
        mpz_urandomm(a, state, nnn); // a is eulers witness
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, nn) != 0) {
            int j = 1;
            while (j <= ss - 1 && mpz_cmp(y, nn) != 0) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
                    return false;
                }
                j += 1;
            }
            if (mpz_cmp(y, nn) != 0) {
                mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
                return false;
            }
        }
    }
    mpz_clears(temp, nn, r, s, temp2, temp3, two, a, y, nnn, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t adder;
    mpz_init(adder);
    mpz_ui_pow_ui(adder, 2, bits);
    while (1) {
        mpz_rrandomb(p, state, bits);
        mpz_add(p, p, adder);
        if (is_prime(p, iters)) {
            break;
        }
    }
    mpz_clear(adder);
}
