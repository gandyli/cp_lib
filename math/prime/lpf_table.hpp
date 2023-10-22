#pragma once
#include "sieve.hpp"

vi lpf_table(int n) {
    auto primes = prime_sieve(n);
    vi r(n + 1, -1);
    _for_r (i, len(primes)) {
        int p = primes[i];
        _for (j, p, n + 1, p)
            r[j] = p;
    }
    return r;
}