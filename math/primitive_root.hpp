#pragma once
#include "math/factorize.hpp"

namespace impl {
    template <Unsigned T>
    T primitive_root(T mod) {
        auto pf = factorize(mod - 1);
        SetMMod(T, mod);
        loop if (T pr = rnd(2, mod); BLK {
                     foreach (p, pf)
                         if (power(mint(pr), (mod - 1) / p) == 1)
                             return false;
                     return true;
                 }) return pr;
    }
} // namespace impl
template <Unsigned T>
constexpr T primitive_root(T mod) {
    if (mod == 2)
        return 1;
    if constexpr (std::is_same_v<T, u32>) {
        if (std::is_constant_evaluated()) {
            u64 ds[32]{};
            int idx = 0;
            u64 m = mod - 1;
            for (u64 i = 2; i * i <= m; i++)
                if (m % i == 0) {
                    ds[idx++] = i;
                    while (m % i == 0)
                        m /= i;
                }
            if (m != 1)
                ds[idx++] = m;
            T pr = 2;
            loop {
                int flg = 1;
                _for (i, idx) {
                    u64 a = pr, b = (mod - 1) / ds[i], r = 1;
                    while (b) {
                        if (b & 1)
                            r = r * a % mod;
                        a = a * a % mod;
                        b >>= 1;
                    }
                    if (r == 1) {
                        flg = 0;
                        break;
                    }
                }
                if (flg == 1)
                    break;
                pr++;
            }
            return pr;
        }
    }
    return impl::primitive_root(mod);
}