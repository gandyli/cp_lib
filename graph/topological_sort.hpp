#pragma once
#include "graph/base.hpp"

std::optional<vi> toposort(const DirectedGraph auto& g) {
    const int n = g.n;
    vc<bool> vis(n), ins(n);
    vi r;
    auto dfs = [&](auto&& dfs, int u) {
        if (ins[u])
            return false;
        if (!vis[u]) {
            ins[u] = true;
            foreach (v, g[u])
                if (!dfs(dfs, v))
                    return false;
            vis[u] = true;
            r.eb(u);
            ins[u] = false;
        }
        return true;
    };
    _for (i, n)
        if (!vis[i] && !dfs(dfs, i))
            return std::nullopt;
    reverse(r);
    return r;
}