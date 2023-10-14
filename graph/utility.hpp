#pragma once
#include "base.hpp"

vi Depth(const UnweightedGraph& g, int s = 0) {
    vi dep(len(g), -1);
    dep[s] = 0;
    std::queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        foreach (v, g[u])
            if (dep[v] == -1) {
                dep[v] = dep[u] + 1;
                q.emplace(v);
            }
    }
    return dep;
}

template <typename T>
Vec<T> Depth(const WeightedGraph<T>& g, int s = 0) {
    Vec<T> dep(len(g), -1);
    auto dfs = [&](auto&& dfs, int u, int f, T d) -> void {
        dep[u] = d;
        foreach (v, g[u])
            if (v != f)
                dfs(dfs, v, u, d + v.cost);
    };
    dfs(dfs, s, -1, 0);
    return dep;
}

auto Diameter(const UnweightedGraph& g) {
    auto d = Depth(g, 0);
    int u = max_element(d) - d.begin();
    d = Depth(g, u);
    int v = max_element(d) - d.begin();
    struct {
        int x, y, len;
    } ret{u, v, d[v]};
    return ret;
}

template <typename T>
auto Diameter(const WeightedGraph<T>& g) {
    auto d = Depth(g, 0);
    int u = max_element(d) - d.begin();
    d = Depth(g, u);
    int v = max_element(d) - d.begin();
    struct {
        int x, y;
        T len;
    } ret{u, v, d[v]};
    return ret;
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