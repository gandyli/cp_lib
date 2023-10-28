#pragma once
#include "graph/base.hpp"

struct TwoEdgeComponentResult {
    int id;
    vi comp;
};
auto two_edge_component(const UndirectedGraph auto& g) {
    int n = g.n, m = g.m;
    vi par(n, -2), dp(n), V;
    V.reserve(n);
    Vec<bool> used(m);
    auto dfs = [&](auto&& dfs, int u) -> void {
        V.eb(u);
        foreach (v, g[u])
            if (!used[v.id]) {
                used[v.id] = true;
                if (par[v] == -2) {
                    par[v] = u;
                    dfs(dfs, v);
                }
                else {
                    dp[u]++;
                    dp[v]--;
                }
            }
    };
    _for (u, n)
        if (par[u] == -2)
            par[u] = -1, dfs(dfs, u);
    _for_r (i, n)
        if (par[V[i]] != -1)
            dp[par[V[i]]] += dp[V[i]];
    int id = 0;
    vi comp(n);
    foreach (u, V)
        comp[u] = !dp[u] ? id++ : comp[par[u]];
    return TwoEdgeComponentResult{id, std::move(comp)};
}