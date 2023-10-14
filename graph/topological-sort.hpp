#pragma once
#include "base.hpp"

vi topologicalSort(auto& g) {
    int n = len(g);
    Vec<bool> vis(n), ins(n);
    vi ret;
    auto dfs = [&](auto&& dfs, int u) {
        if (ins[u])
            return false;
        if (!vis[u]) {
            ins[u] = true;
            foreach (v, g[u])
                if (!dfs(dfs, v))
                    return false;
            vis[u] = true;
            ret.eb(u);
            ins[u] = false;
        }
        return true;
    };
    _for (i, n)
        if (!vis[i] && !dfs(dfs, i))
            return {};
    reverse(ret);
    return ret;
}