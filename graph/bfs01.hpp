#pragma once
#include "base.hpp"

template <typename T>
struct BFS01Result1 {
    Vec<T> dis;
    vi par;
};
template <typename T>
struct BFS01Result2 {
    Vec<T> dis;
    vi par;
    vi root;
};
template <typename T>
auto bfs01(const auto& g, int s = 0) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    std::deque<int> q;
    dis[s] = 0;
    q.eb(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                if (v.cost)
                    q.emplace_back(v);
                else
                    q.emplace_front(v);
            }
    }
    return BFS01Result1{std::move(dis), std::move(par)};
}
template <typename T>
auto bfs01(const auto& g, const vi& s) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    vi root(n, -1);
    std::deque<int> q;
    foreach (s, s) {
        dis[s] = 0;
        root[s] = s;
        q.eb(s);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                root[v] = root[u];
                if (v.cost)
                    q.emplace_back(v);
                else
                    q.emplace_front(v);
            }
    }
    return BFS01Result2{std::move(dis), std::move(par), std::move(root)};
}
