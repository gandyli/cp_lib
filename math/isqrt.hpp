#pragma once
#include "math/fixpoint.hpp"

template <Unsigned T>
T isqrt(T a) {
    if (a < 4)
        return a > 0;
    auto guess = [&](T x) -> T { return sqrt(x); };
    auto next = [&](T x) { return (a / x + x) >> 1; };
    return fixpoint(guess(a), next);
}
template <Signed T>
T isqrt(T a) {
    ASSERT(a >= 0);
    return isqrt<std::make_unsigned_t<T>>(a);
}