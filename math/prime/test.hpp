#pragma once
#include "modint/montgomery.hpp"

constexpr bool is_prime(auto x) {
    auto is_prime = [](auto x) -> bool {
        if (x < 64)
            return 0x28208a20a08a28ac >> x & 1;
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0)
            return false;
        if (x < 121)
            return true;
        using T = decltype(x);
        SetMMod(T, x);
        const T d = (x - 1) >> std::countr_zero(x - 1);
        auto mr = [&](T a) {
            const mint one = 1, mone = -1;
            auto y = power<mint>(a, d);
            T t = d;
            while (y != one && y != mone && t != x - 1)
                y *= y, t <<= 1;
            return y == mone || t % 2;
        };
        if (x < (1ULL << 32))
            return mr(2) && mr(7) && mr(61);
        for (T a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
            if (!mr(a))
                return false;
        return true;
    };
    if (x < (1 << 30))
        return is_prime(u32(x));
    return is_prime(u64(x));
}
