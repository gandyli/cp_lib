#pragma once
#include "template.hpp"

template <typename T>
int ctz(T x) {
    if constexpr (sizeof(T) <= 4)
        return __builtin_ctz(x);
    else if constexpr (sizeof(T) == 8)
        return __builtin_ctzll(x);
    else if (u64(x))
        return ctz(u64(x));
    else
        return 64 + ctz(x >> 64);
}

template <typename T, typename U>
constexpr auto binary_gcd(T _a, U _b) {
    using S = make_signed_t<std::common_type_t<T, U>>;
    S a = _a >= 0 ? _a : -_a, b = _b >= 0 ? _b : -_b;
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    int i = ctz(a);
    int j = ctz(b);
    int k = min(i, j);
    b >>= j;
    while (a != 0) {
        a >>= i;
        S diff = b - a;
        i = ctz(diff);
        chkmin(b, a);
        a = std::abs(diff);
    }
    return b << k;
}