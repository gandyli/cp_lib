#pragma once
#include "graph/bfs01.hpp"
#include "graph/restore_path.hpp"

template <typename T>
struct DiameterResult {
    T diam;
    vi path;
};
template <typename T>
auto diameter(const auto& g) {
    auto [d1, _] = bfs01<T>(g, 0);
    int u = max_element(d1) - d1.begin();
    auto [d2, par] = bfs01<T>(g, u);
    int v = max_element(d2) - d2.begin();
    return DiameterResult{d2[v], restore_path(par, v)};
}

vi get_path(const auto& g, int u, int v) {
    vi r;
    auto dfs = [&](auto&& dfs, int u, int f) -> bool {
        r.eb(u);
        if (u == v)
            return true;
        foreach (v, g[u])
            if (v != f && dfs(dfs, v, u))
                return true;
        r.pop_back();
        return false;
    };
    dfs(dfs, u, -1);
    return r;
}