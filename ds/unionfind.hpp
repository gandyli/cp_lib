#pragma once
#include "template.hpp"

template <bool ByRank = true>
struct UnionFind {
    int n, comp;
    vi a;
    UnionFind(int n = 0) { build(n); }
    void build(int n) {
        this->n = comp = n;
        a.assign(n, -1);
    }
    int get(int x) {
        while (a[x] >= 0) {
            int y = a[a[x]];
            if (y < 0)
                return a[x];
            x = a[x] = y;
        }
        return x;
    }
    int operator[](int x) { return get(x); }
    bool same(int x, int y) { return get(x) == get(y); }
    bool merge(int x, int y) {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        comp--;
        if (ByRank && a[x] > a[y])
            swap(x, y);
        a[x] += a[y];
        a[y] = x;
        return true;
    }
    int size(int x) { return -a[get(x)]; }
};