#pragma once
#include "template.hpp"
#include "montgomery.hpp"

namespace impl {
    bool is_prime_32(u32 x) {
        if (x < 64)
            return 0x28208a20a08a28ac >> x & 1;
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0)
            return false;
        if (x < 121)
            return true;
        using ctx = DynamicMontgomeryReductionContext<u32>;
        auto _guard = ctx::set_mod(x);
        using mint = MontgomeryModInt<ctx>;
        const u32 d = (x - 1) >> __builtin_ctz(x - 1);
        auto mr = [&](u32 a) {
            const mint one = 1, mone = -1;
            auto y = power(mint(a), d);
            u32 t = d;
            while (y != one && y != mone && t != x - 1)
                y *= y, t <<= 1;
            return y == mone || t % 2;
        };
        return mr(2) && mr(7) && mr(61);
    }
    bool is_prime_64(u64 x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0)
            return false;
        using ctx = DynamicMontgomeryReductionContext<u64>;
        auto _guard = ctx::set_mod(x);
        using mint = MontgomeryModInt<ctx>;
        const u64 d = (x - 1) >> __builtin_ctzll(x - 1);
        auto mr = [&](u64 a) {
            const mint one = 1, mone = -1;
            auto y = power(mint(a), d);
            u64 t = d;
            while (y != one && y != mone && t != x - 1)
                y *= y, t <<= 1;
            return y == mone || t % 2;
        };
        if (x < (1ull << 32))
            return mr(2) && mr(7) && mr(61);
        for (u64 a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            if (x <= a)
                return true;
            if (!mr(a))
                return false;
        }
        return true;
    }
} // namespace impl
template <typename T>
bool is_prime(T x) {
    if (x < (1 << 30))
        return impl::is_prime_32(x);
    return impl::is_prime_64(x);
}
