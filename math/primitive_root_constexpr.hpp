#pragma once
#include "template.hpp"

constexpr u32 primitive_root_constexpr(u32 mod) {
    if (mod == 2)
        return 1;
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
    u32 pr = 1;
    loop {
        pr++;
        bool ok = true;
        _for (i, idx) {
            u64 a = pr, b = (mod - 1) / ds[i], r = 1;
            while (b) {
                if (b & 1)
                    r = r * a % mod;
                a = a * a % mod;
                b >>= 1;
            }
            if (r == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return pr;
    }
}