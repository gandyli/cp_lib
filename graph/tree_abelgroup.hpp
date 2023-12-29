#pragma once
#include "ds/fenwicktree.hpp"

template <typename TREE, typename Monoid, bool edge = false, bool path_query = true, bool subtree_query = true>
requires (path_query || subtree_query)
struct Tree_AbelGroup {
    using M = Monoid;
    using X = M::value_type;
    const TREE& tree;
    int n;
    FenwickTree<M> bit_path, bit_subtree;
    Tree_AbelGroup(const TREE& tree): tree(tree) { build(); }
    template <std::convertible_to<X> T>
    Tree_AbelGroup(const TREE& tree, const Vec<T>& a): tree(tree) { build(a); }
    Tree_AbelGroup(const TREE& tree, std::invocable<int> auto&& f): tree(tree) { build(f); }
    void build() {
        build([&](int) { return M::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const Vec<T>& a) {
        build([&](int i) { return a[i]; });
    }
    void build(std::invocable<int> auto&& f) {
        n = tree.n;
        Vec<X> a, b;
        if constexpr (path_query)
            a.resize(n << 1);
        if constexpr (subtree_query)
            b.resize(n);
        _for (i, n) {
            X x;
            if constexpr (edge)
                x = i ? f(tree.v_to_e(i)) : M::unit();
            else
                x = f(i);
            if constexpr (path_query) {
                a[tree.elid(i)] = x;
                a[tree.erid(i)] = M::inverse(x);
            }
            if constexpr (subtree_query)
                b[tree.lid[i]] = x;
        }
        if constexpr (path_query)
            bit_path.build(a);
        if constexpr (subtree_query)
            bit_subtree.build(b);
    }
    void multiply(int i, const X& x) {
        if constexpr (edge)
            i = tree.e_to_v(i);
        if constexpr (path_query) {
            bit_path.multiply(tree.elid(i), x);
            bit_path.multiply(tree.erid(i), M::inverse(x));
        }
        if constexpr (subtree_query)
            bit_subtree.multiply(tree.lid[i], x);
    }
    X prod_path(int u, int v) const
      requires (path_query)
    {
        int lca = tree.lca(u, v);
        return M::op(bit_path.prod(tree.elid(lca) + 1, tree.elid(u) + 1), bit_path.prod(tree.elid(lca) + edge, tree.elid(v) + 1));
    }
    X prod_subtree(int u) const
      requires (subtree_query)
    { return bit_subtree.prod(tree.lid[u] + edge, tree.rid[u]); }
    X prod_all() const
      requires (subtree_query)
    { return bit_subtree.prod_all(); }
};