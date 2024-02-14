#pragma once
#include "utility/memory_pool.hpp"

template <typename T, bool PERSISTENT, int N = -1>
struct Dynamic_Array {
    struct Node {
        T x;
        Node* ch[2];
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    T x0;
    Dynamic_Array(T x0 = {}): x0(std::move(x0)) {}

    np new_node() { return pool.new_node({x0, {nullptr, nullptr}}); }
    np new_node(const vc<T>& a) {
        np u = new_node();
        _for (i, len(a))
            u = set(u, i, a[i], false);
        return u;
    }
    T get(np u, int i) const {
        if (!u)
            return x0;
        if (i == 0)
            return u->x;
        return get(u->ch[i & 1], (i - 1) >> 1);
    }
    np set(np u, int i, T x, bool copy = true) {
        if (!u)
            u = new_node();
        else if (copy && PERSISTENT)
            u = pool.new_node(*u);
        if (i == 0) {
            u->x = std::move(x);
            return u;
        }
        u->ch[i & 1] = set(u->ch[i & 1], (i - 1) >> 1, std::move(x), copy);
        return u;
    }
};