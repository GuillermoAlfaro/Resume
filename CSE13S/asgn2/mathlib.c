#include "mathlib.h"

#include <stdint.h>

double Exp(double x) {
    double sum, k, trm;
    k = 1.0;
    trm = 1.0;
    sum = trm;
    while (trm > EPSILON) {
        trm *= Abs(x) / k;
        sum += trm;
        k += 1;
    }
    if (x > 0) {
        return sum;
    } else {
        return 1 / sum;
    }
}

double Sin(double x) {
    double s, v, t, k;
    s = 1.0;
    v = x;
    t = x;
    k = 3.0;

    while (Abs(t) > EPSILON) {
        t = t * (x * x) / ((k - 1) * k);
        s = -s;
        v += s * t;
        k += 2.0;
    }
    return v;
}

double Cos(double x) {
    double s, v, t, k;
    s = 1.0;
    v = 1.0;
    t = 1.0;
    k = 2.0;
    while (Abs(t) > EPSILON) {
        t = t * (x * x) / ((k - 1) * k);
        s = -s;
        v += s * t;
        k += 2.0;
    }
    return v;
}

double Sqrt(double x) {
    double z, y, f;
    f = 1.0;
    z = 0.0;
    y = 1.0;
    while (x > 1) {
        x /= 4.0;
        f *= 2.0;
    }
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
    }
    return y * f;
}

double Log(double x) {
    double y, p, e, f;
    y = 1.0;
    p = Exp(y);
    f = 0;
    e = Exp(1);
    while (x > e) {
        x = x / e;
        f += 1;
    }
    while (Abs(p - x) > EPSILON) {
        y = y + (x / p) - 1;
        p = Exp(y);
    }
    return y + f;
}

double integrate(double (*f)(double x), double a, double b, uint32_t n) {
    double h, sum, patch;
    int i;
    h = (b - a) / n;
    sum = f(a) + f(b);
    for (i = 1; i < n; i += 1) {
        patch = a + i * h;
        if (patch == 0) {
            patch = 10e-100;
        }
        if (i % 2 != 0) {
            sum += 4 * f(patch);
        } else {
            sum += 2 * f(patch);
        }
    }
    sum *= h / 3;
    return sum;
}
