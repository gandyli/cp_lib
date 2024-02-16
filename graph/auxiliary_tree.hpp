#pragma once
#include "graph/tree.hpp"

template <typename TREE>
auto auxiliary_tree(const TREE& tree, vc<int> V) {
    sort(V, [&](int x, int y) { return tree.lid[x] < tree.lid[y]; });
    int n = len(V);
    _for (i, n - 1)
        V.eb(tree.lca(V[i], V[i + 1]));
    V.eb(tree.lca(V[n - 1], V[0]));
    V.eb(tree.id[0]);
    sort(V, [&](int x, int y) { return tree.lid[x] < tree.lid[y]; });
    V.erase(std::unique(all(V)), V.end());
    n = len(V);
    Graph<typename TREE::cost_type, true> g(n);
    vi st{0};
    _for (i, 1, n) {
        loop {
            int p = V[st.back()];
            if (tree.in_subtree(V[i], p))
                break;
            st.pop_back();
        }
        int p = V[st.back()], u = V[i];
        if constexpr (TREE::graph_type::is_weighted())
            g.add(st.back(), i, tree.wdep[u] - tree.wdep[p]);
        else
            g.add(st.back(), i, tree.dep[u] - tree.dep[p]);
        st.eb(i);
    }
    g.build();
    return std::pair{std::move(g), std::move(V)};
}