#include "functions.h"
#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// usage: ./main [-a] [-b] [-c] [-d] [-e] [-f] [-g] [-h] [-i] [-j] [-n count]

int main(int argc, char **argv) {
    int opt = 0;
    long count = 100;
    bool aa = false;
    bool bb = false;
    bool cc = false;
    bool dd = false;
    bool ee = false;
    bool ff = false;
    bool gg = false;
    bool hh = false;
    bool ii = false;
    bool jj = false;
    double p = -47.1242;
    double q = -47.1242;
    bool H = false;

    while ((opt = getopt(argc, argv, "abcdefghijn:p:q:H")) != -1) {
        switch (opt) {
        case 'a': aa = true; break;
        case 'b': bb = true; break;
        case 'c': cc = true; break;
        case 'd': dd = true; break;
        case 'e': ee = true; break;
        case 'f': ff = true; break;
        case 'g': gg = true; break;
        case 'h': hh = true; break;
        case 'i': ii = true; break;
        case 'j': jj = true; break;
        case 'n': count = strtol(optarg, NULL, 0); break;
        case 'p': p = strtod(optarg, NULL); break;
        case 'q': q = strtod(optarg, NULL); break;
        case 'H': H = true; break;
        default: H = true; break;
        }
    }
    if ((aa || bb || cc || dd || ee || ff || gg || hh || ii || jj) == false) {
        printf("Missing integral\n");
        H = true;
    }
    if (p == -47.1242) {
        H = true;
        printf("Missing Low\n");
    }
    if (q == -47.1242) {
        H = true;
        printf("Missing High\n");
    }
    if (H) {
        int s;
        FILE *file;
        file = fopen("usageOptions.txt", "r");
        while ((s = fgetc(file)) != EOF) {
            printf("%c", s);
        }
        fclose(file);
        return 0;
    }
    if (aa) {
        printf("sqrt(1 - x^4) , %lf, %lf, %ld\n", p, q, count);
    }
    if (bb) {
        printf("1/log(x) , %lf, %lf, %ld\n", p, q, count);
    }
    if (cc) {
        printf("e^(-x^2) , %lf, %lf, %ld\n", p, q, count);
    }
    if (dd) {
        printf("sin(x^2) , %lf, %lf, %ld\n", p, q, count);
    }
    if (ee) {
        printf("cos(x^2) , %lf, %lf, %ld\n", p, q, count);
    }
    if (ff) {
        printf("log(log(x)) , %lf, %lf, %ld\n", p, q, count);
    }
    if (gg) {
        printf("sin(x)/x , %lf, %lf, %ld\n", p, q, count);
    }
    if (hh) {
        printf("e^(-x)/x , %lf, %lf, %ld\n", p, q, count);
    }
    if (ii) {
        printf("e^(e^x) , %lf, %lf, %ld\n", p, q, count);
    }
    if (jj) {
        printf("sqrt(sin^2(x) + cos^2(x)) , %lf, %lf, %ld\n", p, q, count);
    }
    for (int k = 2; k <= count; k += 2) {
        if (aa) {
            printf("%d, %.15g\n", k, integrate(a, p, q, k));
        }
        if (bb) {
            printf("%d, %.15g\n", k, integrate(b, p, q, k));
        }
        if (cc) {
            printf("%d, %.15g\n", k, integrate(c, p, q, k));
        }
        if (dd) {
            printf("%d, %.15g\n", k, integrate(d, p, q, k));
        }
        if (ee) {
            printf("%d, %.15g\n", k, integrate(e, p, q, k));
        }
        if (ff) {
            printf("%d, %.15g\n", k, integrate(f, p, q, k));
        }
        if (gg) {
            printf("%d, %.15g\n", k, integrate(g, p, q, k));
        }
        if (hh) {
            printf("%d, %.15g\n", k, integrate(h, p, q, k));
        }
        if (ii) {
            printf("%d, %.15g\n", k, integrate(i, p, q, k));
        }
        if (jj) {
            printf("%d, %.15g\n", k, integrate(j, p, q, k));
        }
    }
    return 1;
}
