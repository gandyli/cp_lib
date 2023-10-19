#pragma once
#include "bfs01.hpp"
#include "restore_path.hpp"

template <typename T>
std::pair<T, vi> diameter(const Graph<int, 0>& g) {
    auto [d1, _] = bfs01<T>(g, 0);
    int u = max_element(d1) - d1.begin();
    auto [d2, par] = bfs01<T>(g, u);
    int v = max_element(d2) - d2.begin();
    return {d2[v], restore_path(par, v)};
}

vi Path(auto&& g, int u, int v) {
    vi ret;
    auto dfs = [&](auto&& dfs, int u, int f) -> bool {
        ret.eb(u);
        if (u == v)
            return true;
        foreach (v, g[u])
            if (v != f && dfs(dfs, v, u))
                return true;
        ret.pop_back();
        return false;
    };
    dfs(dfs, u, -1);
    return ret;
}