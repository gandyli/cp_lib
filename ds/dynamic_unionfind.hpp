#pragma once
#include "ds/dynamic_array.hpp"

template <bool PERSISTENT, int N = -1>
struct Dynamic_UnionFind {
    Dynamic_Array<int, PERSISTENT, N> a;
    using np = decltype(a)::np;
    Dynamic_UnionFind(): a(-1) {}
    np new_node() { return a.new_node(); }
    int get(np u, int x) {
        loop {
            int p = a.get(u, x);
            if (p < 0)
                return x;
            x = p;
        }
    }
    bool same(np u, int x, int y) { return get(u, x) == get(u, y); }
    std::pair<np, bool> merge(np u, int x, int y) {
        x = get(u, x), y = get(u, y);
        if (x == y)
            return {u, false};
        if (a.get(u, x) > a.get(u, y))
            swap(x, y);
        int sz = a.get(u, x) + a.get(u, y);
        u = a.set(u, x, sz);
        u = a.set(u, y, x);
        return {u, true};
    }
};