#pragma once
#include "graph/base.hpp"

std::optional<vi> toposort(const DirectedGraph auto& g) {
    const int n = g.n;
    vcb vis(n), ins(n);
    vi r;
    auto dfs = [&](auto&& dfs, int u) {
        if (ins[u])
            return false;
        if (!vis[u]) {
            ins[u] = 1;
            foreach (v, g[u])
                if (!dfs(dfs, v))
                    return false;
            vis[u] = 1;
            r.eb(u);
            ins[u] = 0;
        }
        return true;
    };
    _for (i, n)
        if (!vis[i] && !dfs(dfs, i))
            return std::nullopt;
    reverse(r);
    return r;
}