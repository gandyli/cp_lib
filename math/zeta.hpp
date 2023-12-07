#pragma once
#include "math/prime/sieve.hpp"

template <typename T>
void divisor_zeta(Vec<T>& a) {
    ASSERT(a[0] == 0);
    int n = len(a) - 1;
    foreach (p, prime_sieve(n))
        _for (x, 1, n / p + 1)
            a[p * x] += a[x];
}

template <typename T>
void divisor_mobius(Vec<T>& a) {
    ASSERT(a[0] == 0);
    int n = len(a) - 1;
    foreach (p, prime_sieve(n))
        _for_r (x, 1, n / p + 1)
            a[p * x] -= a[x];
}

template <typename T>
void multiplier_zeta(Vec<T>& a) {
    ASSERT(a[0] == 0);
    int n = len(a) - 1;
    foreach (p, prime_sieve(n))
        _for_r (x, 1, n / p + 1)
            a[x] += a[p * x];
}

template <typename T>
void multiplier_mobius(Vec<T>& a) {
    ASSERT(a[0] == 0);
    int n = len(a) - 1;
    foreach (p, prime_sieve(n))
        _for (x, 1, n / p + 1)
            a[x] -= a[p * x];
}