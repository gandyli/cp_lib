#pragma once
#include "monoid/monoid_reverse.hpp"
#include "ds/disjoint_sparse_table.hpp"

template <typename TREE, typename Monoid, bool edge = false>
struct Static_Tree_Monoid {
    using M = Monoid;
    using X = M::value_type;

    const TREE& tree;
    int n;
    Disjoint_Sparse_Table<M> dst;
    Disjoint_Sparse_Table<Monoid_Reverse<M>> rdst;
    Static_Tree_Monoid(const TREE& tree): tree(tree) { build(); }
    Static_Tree_Monoid(const TREE& tree, const Vec<X>& a): tree(tree) { build(a); }
    Static_Tree_Monoid(const TREE& tree, auto&& f): tree(tree) { build(f); }
    void build() {
        build([&](int) { return M::unit(); });
    }
    void build(const Vec<X>& a) {
        build([&](int i) { return a[i]; });
    }
    void build(auto&& f) {
        n = tree.n;
        if constexpr (edge) {
            auto fe = [&](int i) { return i ? f(tree.v_to_e(tree.id[i])) : M::unit(); };
            dst.build(n, fe);
            if constexpr (!M::commute)
                rdst.build(n, fe);
        }
        else {
            auto fv = [&](int i) { return f(tree.id[i]); };
            dst.build(n, fv);
            if constexpr (!M::commute)
                rdst.build(n, fv);
        }
    }
    X prod(int x, int y) const {
        if constexpr (M::commute)
            return x <= y ? dst.prod(x, y + 1) : dst.prod(y, x + 1);
        else
            return x <= y ? dst.prod(x, y + 1) : rdst.prod(y, x + 1);
    }
    X prod_path(int u, int v) const {
        X r = M::unit();
        for (auto&& [x, y]: tree.path_decomposition(u, v, edge))
            r = M::op(r, prod(x, y));
        return r;
    }
    X prod_subtree(int u) const { return dst.prod(tree.lid[u] + edge, tree.rid[u]); }
    X prod_all() const { return prod_subtree(tree.id[0]); }
    int max_path(auto&& check, int u, int v) {
        X r = M::unit();
        auto check0 = [&](const X& x) { return check(M::op(r, x)); };
        if constexpr (edge) {
            if (!check(M::unit()))
                return -1;
            int lca = tree.lca(u, v);
            for (auto&& [x, y]: tree.path_decomposition(u, lca, edge)) {
                ASSERT(x >= y);
                X t = prod(x, y);
                if (check(M::op(r, t))) {
                    r = M::op(r, t);
                    u = tree.fa[tree.id[y]];
                }
                else {
                    int p;
                    if constexpr (M::commute)
                        p = dst.min_left(check0, x + 1);
                    else
                        p = rdst.min_left(check0, x + 1);
                    return p == x + 1 ? u : tree.fa[tree.id[p]];
                }
            }
            for (auto&& [x, y]: tree.path_decomposition(lca, v, edge)) {
                ASSERT(x <= y);
                X t = prod(x, y);
                if (check(M::op(r, t))) {
                    r = M::op(r, t);
                    v = tree.id[y];
                }
                else {
                    int p = dst.max_right(check0, x);
                    return p == x ? u : tree.id[p - 1];
                }
            }
        }
        else {
            if (!check(prod(u, u)))
                return -1;
            for (auto&& [x, y]: tree.path_decomposition(u, v, edge)) {
                X t = prod(x, y);
                if (check(M::op(r, t))) {
                    r = M::op(r, t);
                    u = tree.id[y];
                }
                else {
                    if (x <= y) {
                        int p = dst.max_right(check0, x);
                        return p == x ? u : tree.id[p - 1];
                    }
                    int p;
                    if constexpr (M::commute)
                        p = dst.min_left(check0, x + 1);
                    else
                        p = rdst.min_left(check0, x + 1);
                    return p == x + 1 ? u : tree.id[p];
                }
            }
        }
        return v;
    }
};