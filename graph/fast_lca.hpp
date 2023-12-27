#pragma once
#include "ds/static_range_product.hpp"
#include "ds/sparse_table.hpp"
#include "monoid/min.hpp"
#include "graph/tree.hpp"

template <typename G>
struct Fast_Lca_Tree: Tree<G> {
    Static_Range_Product<Sparse_Table, Monoid_Min<int>> st;
    vi pos;
    using Tree<G>::n, Tree<G>::lca;
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
};