#pragma once
#include "template.hpp"

template <typename T, typename U>
constexpr auto binary_gcd(T _a, U _b) {
    using S = make_signed_t<std::common_type_t<T, U>>;
    S a = _a >= 0 ? _a : -_a, b = _b >= 0 ? _b : -_b;
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    int i = std::__countr_zero(a);
    int j = std::__countr_zero(b);
    int k = min(i, j);
    b >>= j;
    while (a != 0) {
        a >>= i;
        S diff = b - a;
        i = std::__countr_zero(diff);
        chkmin(b, a);
        a = max(diff, -diff);
    }
    return b << k;
}