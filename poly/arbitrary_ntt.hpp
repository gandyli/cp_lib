#pragma once
#include "modint/montgomery.hpp"

namespace ArbitraryNTT {
    constexpr u32 m0 = 167772161;
    constexpr u32 m1 = 469762049;
    constexpr u32 m2 = 754974721;
    using mint0 = MMInt<m0>;
    using mint1 = MMInt<m1>;
    using mint2 = MMInt<m2>;
    constexpr u32 r01 = mint1(m0).inv().val();
    constexpr u32 r02 = mint2(m0).inv().val();
    constexpr u32 r12 = mint2(m1).inv().val();
    constexpr u32 r02r12 = u64(r02) * r12 % m2;
    constexpr u64 w1 = m0;
    constexpr u64 w2 = u64(m0) * m1;
    template <typename T>
    void crt(auto&& c0, auto&& c1, auto&& c2, auto&& r, u64 w1, u64 w2) {
        _for (i, len(r)) {
            u64 n1 = val(c1[i]), n2 = val(c2[i]), a = val(c0[i]);
            u64 b = (n1 + m1 - a) * r01 % m1;
            u64 c = ((n2 + m2 - a) * r02r12 + (m2 - b) * r12) % m2;
            r[i] = a + b * w1 + T(c) * w2;
        }
    }
} // namespace ArbitraryNTT