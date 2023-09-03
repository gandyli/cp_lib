#pragma once

#include "template.hpp"

struct DSU {
    vi f, siz;
    int comp;
    DSU(int n): f(n), siz(n, 1), comp(n) { std::iota(all(f), 0); }
    int leader(int x) {
        while (x != f[x])
            x = f[x] = f[f[x]];
        return x;
    }
    bool same(int x, int y) {
        return leader(x) == leader(y);
    }
    bool merge(int x, int y) {
        x = leader(x);
        y = leader(y);
        if (x == y)
            return false;
        comp--;
        if (siz[x] < siz[y])
            std::swap(x, y);
        siz[x] += siz[y];
        siz[y] = 0;
        f[y] = x;
        return true;
    }
    int size(int x) {
        return siz[leader(x)];
    }
};