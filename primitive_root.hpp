#pragma once
#include "template.hpp"

constexpr u32 get_pr(u32 mod) {
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

    u32 pr = 2;
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
};
