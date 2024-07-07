#pragma once
#include "template.hpp"

// Enumerates intervals [l, r) of x such that L <= (x ^ a) < R.
void xor_range(u64 a, u64 l, u64 r, auto&& f) {
    _for (i, 64) {
        if (l == r)
            break;
        u64 t = 1ULL << i;
        if (l & t) {
            f(l ^ a, (l ^ a) + t);
            l += t;
        }
        if (r & t) {
            f((r - t) ^ a, ((r - t) ^ a) + t);
            r -= t;
        }
        if (a & t)
            a ^= t;
    }
}