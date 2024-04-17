#pragma once
#include "ds/sparse_table.hpp"
#include "monoid/min.hpp"
#include "graph/tree.hpp"

template <typename G>
struct Fast_Lca_Tree: Tree<G> {
    Sparse_Table<Monoid_Min<int>> st;
    vi pos;
    using Tree<G>::n;
    Fast_Lca_Tree(G& g, int root = 0): Tree<G>(g, root), pos(n) {
        vi a(n << 1);
        _for (i, n) {
            int x = this->elid(i), y = this->erid(i);
            pos[i] = x;
            a[x] = this->lid[i];
            a[y] = this->lid[this->fa[i]];
        }
        a.back() = -1;
        st.build(a);
    }
    int lca(int u, int v) const {
        u = pos[u], v = pos[v];
        if (u > v)
            swap(u, v);
        return this->id[st.prod(u, v + 1)];
    }
    int lca(int u, int v, int r) const { return lca(u, v) ^ lca(u, r) ^ lca(v, r); }
    int dist(int u, int v) const { return this->dep[u] + this->dep[v] - this->dep[lca(u, v)] * 2; }
    auto wdist(int u, int v) const requires (G::is_weighted())
    { return this->wdep[u] + this->wdep[v] - this->wdep[lca(u, v)] * 2; }
};