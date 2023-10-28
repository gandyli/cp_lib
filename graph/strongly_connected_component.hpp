#pragma once
#include "graph/base.hpp"

struct StronglyConnectedComponentResult {
    int scc;
    vi comp;
};
auto strongly_connected_component(const DirectedGraph auto& g) {
    int n = g.n;
    vi comp(n), low(n), dfn(n, -1);
    vi st;
    st.reserve(n);
    int id = 0, scc = 0;
    auto dfs = [&](auto&& dfs, int u) -> void {
        st.eb(u);
        dfn[u] = low[u] = id++;
        foreach(v, g[u])
            if (dfn[v] == -1) {
                dfs(dfs, v);
                chkmin(low[u], low[v]);
            }
            else
                chkmin(low[u], dfn[v]);
        if (low[u] == dfn[u]) {
            loop {
                int t = pop(st);
                dfn[t] = n;
                comp[t] = scc;
                if (t == u)
                    break;
            }
            scc++;
        }
    };
    _for (u, n)
        if (dfn[u] == -1)
            dfs(dfs, u);
    _for (u, n)
        comp[u] = scc - comp[u] - 1;
    return StronglyConnectedComponentResult{scc, std::move(comp)};
}
auto scc_dag(const DirectedGraph auto& g, int scc, const vi& comp) {
    vvi edges(scc);
    foreach (e, g.edges) {
        int u = comp[e.from], v = comp[e.to];
        if (u != v)
            edges[u].eb(v);
    }
    Graph<int, true> dag(scc);
    _for (i, scc) {
        UNIQUE(edges[i]);
        foreach (to, edges[i])
            dag.add(i, to);
    }
    dag.build();
    return dag;
}
auto scc_dag_weighted(const DirectedGraph auto& g, int scc, const vi& comp) {
    std::decay_t<decltype(g)> dag(scc);
    foreach (e, g.edges) {
        int u = comp[e.from], v = comp[e.to];
        if (u != v)
            dag.add(u, v, e.cost);
    }
    dag.build();
    return dag;
}