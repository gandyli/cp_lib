#pragma once
#include "ds/rollback_array.hpp"

template <bool ByRank = true>
struct Rollback_UnionFind {
    Rollback_Array<int> a;
    Rollback_UnionFind(int n): a(n, -1) {}
    int get(int x) const {
        while (a[x] >= 0)
            x = a[x];
        return x;
    }
    int operator[](int x) const { return get(x); }
    bool same(int x, int y) const { return get(x) == get(y); }
    bool merge(int x, int y) {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        if (ByRank && a[x] > a[y])
            swap(x, y);
        a.set(x, a[x] + a[y]);
        a.set(y, x);
        return true;
    }
    int size(int x) const { return -a[get(x)]; }
    int time() const { return a.time(); }
    void rollback(int t) { a.rollback(t); }
};