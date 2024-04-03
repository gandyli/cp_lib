#pragma once
#include "graph/base.hpp"

Graph<> block_cut_tree(const UndirectedGraph auto& g) {
    const int n = g.n;
    vi low(n), dfn(n, -1), st;
    st.reserve(n);
    int m = n;
    int id = 0;
    vc<pi> edges;
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
                        while (len(st) > cur)
                            edges.eb(m, pop(st));
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
    Graph tree(m, len(edges));
    foreach (a, b, edges)
        tree.add(a, b);
    tree.build();
    return tree;
}