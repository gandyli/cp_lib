#pragma once
#include "graph/base.hpp"

template <typename T>
struct DijkstraResult1 {
    Vec<T> dis;
    vi par;
};
template <typename T>
struct DijkstraResult2 {
    Vec<T> dis;
    vi par;
    vi root;
};
template <typename T>
auto dijkstra_dense(const auto& g, int s = 0) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    Vec<bool> vis(n);
    dis[s] = 0;
    loop {
        int u = -1;
        _for (i, n)
            if (!vis[i] && (u == -1 || dis[i] < dis[u]))
                u = i;
        if (u == -1)
            break;
        vis[u] = true;
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost))
                par[v] = u;
    }
    return DijkstraResult1{std::move(dis), std::move(par)};
}
template <typename T>
auto dijkstra(const auto& g, int s = 0) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    PQG<std::pair<T, int>> q;
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        auto [d, u] = q.top();
        q.pop();
        if (d > dis[u])
            continue;
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                q.emplace(dis[v], v);
            }
    }
    return DijkstraResult1{std::move(dis), std::move(par)};
}
template <typename T>
auto dijkstra(const auto& g, const vi& s) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    vi root(n, -1);
    std::priority_queue<std::pair<T, int>, Vec<std::pair<T, int>>, std::greater<>> q;
    foreach (s, s) {
        dis[s] = 0;
        root[s] = s;
        q.emplace(0, s);
    }
    while (!q.empty()) {
        auto [d, u] = q.top();
        q.pop();
        if (d > dis[u])
            continue;
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                root[v] = root[u];
                q.emplace(dis[v], v);
            }
    }
    return DijkstraResult2{std::move(dis), std::move(par), std::move(root)};
}
