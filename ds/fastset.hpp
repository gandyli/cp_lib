#pragma once
#include "template.hpp"

struct FastSet {
    static constexpr u32 B = 64;
    u32 n, lg;
    Vec<Vec<u64>> a;
    FastSet(u32 n): n(n) {
        do {
            a.eb(n / B + 1);
            n = ceil(n, B);
        } while (n > 1);
        lg = len(a);
    }
    bool contains(u32 x) const {
        return a[0][x / B] >> (x % B) & 1;
    }
    void insert(u32 x) {
        _for (i, lg) {
            a[i][x / B] |= 1ULL << (x % B);
            x /= B;
        }
    }
    void erase(u32 x) {
        _for (i, lg) {
            a[i][x / B] &= ~(1ULL << (x % B));
            if (a[i][x / B])
                break;
            x /= B;
        }
    }
    int prev(u32 x) const {
        if (x >= n)
            x = n - 1;
        _for (i, lg) {
            u64 d = a[i][x / B] << (B - x % B - 1);
            if (!d) {
                if (x < B)
                    break;
                x = x / B - 1;
                continue;
            }
            x -= __builtin_clzll(d);
            _for_r (j, int(i)) {
                x *= B;
                x += B - __builtin_clzll(a[j][x / B]) - 1;
            }
            return x;
        }
        return -1;
    }
    int next(u32 x) const {
        if (x >= n)
            return n;
        _for (i, lg) {
            if (x / B == len(a[i]))
                break;
            u64 d = a[i][x / B] >> (x % B);
            if (!d) {
                x = x / B + 1;
                continue;
            }
            x += __builtin_ctzll(d);
            _for_r (j, int(i)) {
                x *= B;
                x += __builtin_ctzll(a[j][x / B]);
            }
            return x;
        }
        return n;
    }
    void enumerate(u32 l, u32 r, auto&& f) {
        u32 x = l - 1;
        loop {
            x = next(x + 1);
            if (x >= r)
                break;
            f(x);
        }
    }
};