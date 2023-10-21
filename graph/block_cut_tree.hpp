#pragma once
#include "base.hpp"

Graph<int> block_cut_tree(const UndirectedGraph auto& g) {
    int n = g.n;
    vi low(n), dfn(n, -1), st;
    st.reserve(n);
    int m = n;
    int id = 0;
    Vec<pi> edges;
    auto dfs = [&](auto&& dfs, int u, int f) -> void {
        st.eb(u);
        dfn[u] = low[u] = id++;
        int child = 0;
        foreach (v, g[u])
            if (v != f) {
                if (dfn[v] == -1) {
                    child++;
                    int cur = len(st);
                    dfs(dfs, v, u);
                    chkmin(low[u], low[v]);
                    if ((f == -1 && child > 1) || (f != -1 && low[v] >= dfn[u])) {
                        edges.eb(m, u);
                        while (len(st) > cur) {
                            edges.eb(m, st.back());
                            st.pop_back();
                        }
                        m++;
                    }
                }
                else
                    chkmin(low[u], dfn[v]);
            }
    };
    _for (u, n)
        if (dfn[u] == -1) {
            dfs(dfs, u, -1);
            foreach (x, st)
                edges.eb(m, x);
            m++;
            st.clear();
        }
    Graph<int> tree(m, len(edges));
    for (auto&& [a, b]: edges)
        tree.add(a, b);
    tree.build();
    return tree;
}