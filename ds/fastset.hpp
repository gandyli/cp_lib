#pragma once
#include "template.hpp"

struct FastSet {
    static constexpr u32 B = 64;
    u32 n, lg;
    vvc<u64> a;
    FastSet() = default;
    FastSet(u32 n) { build(n); }
    FastSet(u32 n, std::invocable<u32> auto&& f) { build(n, f); }
    void build(u32 n) {
        a.clear();
        this->n = n;
        do {
            a.eb(ceil(n, B));
            n = ceil(n, B);
        } while (n > 1);
        lg = len(a);
    }
    void build(u32 n, std::invocable<u32> auto&& f) {
        build(n);
        _for (i, n)
            a[0][i / B] |= u64(f(i)) << (i % B);
        _for (i, lg - 1)
            _for (j, len(a[i]))
                a[i + 1][j / B] |= u64(!!a[i][j]) << (j % B);
    }
    bool contains(u32 x) const { return a[0][x / B] >> (x % B) & 1; }
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
        chkmin(x, n - 1);
        _for (i, lg) {
            u64 d = a[i][x / B] << (B - x % B - 1);
            if (!d) {
                if (x < B)
                    break;
                x = x / B - 1;
                continue;
            }
            x -= __builtin_clzll(d);
            _for_r (j, i)
                x = x * B + B - __builtin_clzll(a[j][x]) - 1;
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
            _for_r (j, i)
                x = x * B + __builtin_ctzll(a[j][x]);
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