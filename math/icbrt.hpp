#pragma once
#include "math/fixpoint.hpp"

template <Unsigned T>
T icbrt(T a) {
    if (a < 8)
        return a > 0;
    if (a <= u32(-1)) {
        T x = a;
        T y2 = 0;
        T y = 0;
        T smax = std::numeric_limits<T>::digits / 3;
        _for_r (s, smax + 1) {
            T t = s * 3;
            y2 *= 4;
            y *= 2;
            T b = 3 * (y2 + y) + 1;
            if ((x >> t) >= b) {
                x -= b << t;
                y2 += 2 * y + 1;
                y += 1;
            }
        }
        return y;
    }
    auto guess = [&](T x) -> T { return cbrt(x); };
    auto next = [&](T x) { return (a / (x * x) + 2 * x) / 3; };
    return fixpoint(guess(a), next);
}
template <Signed T>
T icbrt(T a) {
    if (a >= 0)
        return icbrt<std::make_signed_t<T>>(a);
    return -icbrt<std::make_signed_t<T>>(-a);
}