#pragma once
#include "template.hpp"

struct Bit_Vector {
    vc<std::pair<u32, u32>> a;
    Bit_Vector(int n): a((n + 63) >> 5) {}
    void set(int i) { a[i >> 5].first |= 1U << (i & 31); }
    void reset(int i) { a[i >> 5].first &= ~(1U << (i & 31)); }
    void build() {
        _for (i, len(a) - 1)
            a[i + 1].second = a[i].second + std::popcount(a[i].first);
    }
    u32 rank1(int i) const {
        auto [x, y] = a[i >> 5];
        return y + std::popcount(x & ((1U << (i & 31)) - 1));
    }
    u32 rank1(int l, int r) const { return rank1(r) - rank1(l); }
    u32 rank0(int i) const { return i - rank1(i); }
    u32 rank0(int l, int r) const { return rank0(r) - rank0(l); }
    u32 rank(int i, bool b) const { return b ? rank1(i) : rank0(i); }
    u32 rank(int l, int r, bool b) const { return b ? rank1(l, r) : rank0(l, r); }
};