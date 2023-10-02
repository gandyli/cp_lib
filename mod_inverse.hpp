#pragma once
#include "template.hpp"

template <std::unsigned_integral T>
constexpr std::tuple<std::make_signed_t<T>, std::make_signed_t<T>, T> bezout(T x, T y) {
    bool swap = x < y;
    if (swap)
        std::swap(x, y);
    if (y == 0) {
        if (x == 0)
            return {0, 0, 0};
        if (swap)
            return {0, 1, x};
        return {1, 0, x};
    }
    using S = std::make_signed_t<T>;
    S s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    loop {
        auto [q, r] = divmod(x, y);
        if (r == 0) {
            if (swap)
                return {t1, s1, y};
            return {s1, t1, y};
        }
        S s2 = s0 - S(q) * s1, t2 = t0 - S(q) * t1;
        x = y;
        y = r;
        s0 = s1;
        s1 = s2;
        t0 = t1;
        t1 = t2;
    }
}
template <std::unsigned_integral T>
constexpr T mod_inverse(T x, T m) {
    auto [s, t, g] = bezout(x, m);
    assert(g == 1);
    return s < 0 ? T(s) + m : s;
}