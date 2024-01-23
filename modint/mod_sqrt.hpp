#pragma once
#include "modint/montgomery.hpp"

template <Modint mint>
std::optional<mint> mod_sqrt(mint a) {
    if (a < 2)
        return a;
    auto k = (mint::mod() - 1) >> 1;
    if (power(a, k) != 1)
        return std::nullopt;
    mint b = 1, D = 1 - a;
    while (power(D, k) == 1)
        ++b, D = b * b - a;
    k++;
    mint f0 = b, f1 = 1, g0 = 1, g1 = 0;
    while (k) {
        if (k & 1)
            std::tie(g0, g1) = std::pair{f0 * g0 + D * f1 * g1, f0 * g1 + f1 * g0};
        std::tie(f0, f1) = std::pair{f0 * f0 + D * f1 * f1, 2 * f0 * f1};
        k >>= 1;
    }
    return g0;
}
template <Unsigned T>
std::optional<T> mod_sqrt(T a, T p) {
    if (a < 2)
        return a;
    SetMMod(T, p);
    auto r = mod_sqrt<mint>(a);
    if (r)
        return r->val();
    return std::nullopt;
}