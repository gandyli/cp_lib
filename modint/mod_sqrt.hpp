#pragma once
#include "modint/montgomery.hpp"

template <Unsigned T>
std::make_signed_t<T> mod_sqrt(T a, T p) {
    if (a < 2)
        return a;
    SetMMod(T, p);
    mint A = a;
    T k = (p - 1) >> 1;
    if (power(A, k) != 1)
        return -1;
    mint b = 1, D = 1 - A;
    while (power(D, k) == 1)
        ++b, D = b * b - A;
    k++;
    mint f0 = b, f1 = 1, g0 = 1, g1 = 0;
    while (k) {
        if (k & 1)
            std::tie(g0, g1) = std::pair{f0 * g0 + D * f1 * g1, f0 * g1 + f1 * g0};
        std::tie(f0, f1) = std::pair{f0 * f0 + D * f1 * f1, 2 * f0 * f1};
        k >>= 1;
    }
    return g0.val();
}