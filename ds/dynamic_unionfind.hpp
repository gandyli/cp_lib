#pragma once
#include "ds/dynamic_array.hpp"

template <bool PERSISTENT, int N = -1>
struct Dynamic_UnionFind {
    Dynamic_Array<int, PERSISTENT, N> a;
    using Node = Dynamic_Array<int, PERSISTENT, N>::Node;
    Dynamic_UnionFind(): a(-1) {}
    Node* new_node() { return a.new_node(); }
    int get(Node* u, int x) {
        loop {
            int p = a.get(u, x);
            if (p < 0)
                return x;
            x = p;
        }
    }
    bool same(Node* u, int x, int y) { return get(u, x) == get(u, y); }
    std::pair<Node*, bool> merge(Node* u, int x, int y) {
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