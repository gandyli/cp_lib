#pragma once

#include "template.hpp"

struct Barrett {
    u32 m{};
    u64 im{};
    Barrett() = default;
    Barrett(int n): m(n), im(u64(-1) / m + 1) {}
    [[nodiscard]] constexpr i64 quo(u64 n) const {
        u64 x = u64((u128(n) * im) >> 64);
        u32 r = n - x * m;
        return m <= r ? x - 1 : x;
    }
    [[nodiscard]] constexpr i64 rem(u64 n) const {
        u64 x = u64((u128(n) * im) >> 64);
        u32 r = n - x * m;
        return m <= r ? r + m : r;
    }
    [[nodiscard]] constexpr std::pair<i64, int> quorem(u64 n) const {
        u64 x = u64((u128(n) * im) >> 64);
        u32 r = n - x * m;
        if (m <= r)
            return {x - 1, r + m};
        return {x, r};
    }
    [[nodiscard]] constexpr i64 pow(u64 n, i64 p) const {
        u32 a = rem(n), r = m != 1;
        while (p) {
            if (p & 1)
                r = rem(u64(r) * a);
            a = rem(u64(a) * a);
            p >>= 1;
        }
        return r;
    }
};