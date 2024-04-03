#pragma once
#include "math/factorize.hpp"
#include "math/zeta.hpp"

template <Unsigned T>
T euler_phi(T n) {
    foreach (p, _, factorize_pair(n))
        n -= n / p;
    return n;
}
vi euler_phi_table(int n) {
    vi a(n + 1);
    iota(all(a), 0);
    divisor_mobius(a);
    return a;
}