#pragma once
#include "template.hpp"

template <typename T, typename U>
auto binary_gcd(T _a, U _b) {
    using CU = make_unsigned_t<std::common_type_t<T, U>>;
    CU a = _a >= 0 ? _a : -_a, b = _b >= 0 ? _b : -_b;
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    int i = std::__countr_zero(a);
    a >>= i;
    int j = std::__countr_zero(b);
    b >>= j;
    int k = min(i, j);
    loop {
        if (a > b)
            swap(a, b);
        b -= a;
        if (b == 0)
            return a << k;
        b >>= std::__countr_zero(b);
    }
}