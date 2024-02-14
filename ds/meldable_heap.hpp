#pragma once
#include "utility/memory_pool.hpp"

template <typename T, bool PERSISTENT, int N = -1, typename Compare = std::less<>>
struct Meldable_Heap: public Compare {
    struct Node {
        Node *l, *r;
        T x;
        int s;
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    Meldable_Heap() = default;
    Meldable_Heap(const Compare& cmp): Compare(cmp) {}
    np new_node(const T& x) { return pool.new_node({nullptr, nullptr, x, 1}); }
    np copy_node(np a) {
        if (a && PERSISTENT)
            return pool.new_node(*a);
        return a;
    }
    np meld(np a, np b) {
        if (!a)
            return b;
        if (!b)
            return a;
        a = copy_node(a);
        b = copy_node(b);
        if ((*this)(b->x, a->x))
            swap(a, b);
        a->r = a->r ? meld(a->r, b) : b;
        if (!a->l || a->l->s < a->r->s)
            swap(a->l, a->r);
        a->s = (a->r ? a->r->s : 0) + 1;
        return a;
    }
    np push(np a, const T& x) { return meld(a, new_node(x)); }
    np pop(np a) { return meld(a->l, a->r); }
    static T top(np a) { return a->x; }
    static vc<T> get_all(np a) {
        vc<T> r;
        auto dfs = [&](auto&& dfs, np a) {
            if (!a)
                return;
            r.eb(a->x);
            dfs(dfs, a->l);
            dfs(dfs, a->r);
        };
        dfs(dfs, a);
        return r;
    }
};