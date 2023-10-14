#include "../graph/base.hpp"

template <typename T>
std::pair<Vec<T>, vi> dijkstra_dense(WeightedGraph<T>& g, int s = 0) {
    Vec dis(len(g), T(-1));
    vi par(len(g), -1);
    Vec<bool> vis(len(g));
    dis[s] = 0;
    loop {
        int u = -1;
        _for (i, len(g))
            if (!vis[i] && (u == -1 || dis[i] < dis[u]))
                u = i;
        if (u == -1)
            break;
        vis[u] = true;
        foreach (v, g[u])
            if (dis[v] == T(-1) || dis[u] + v.cost < dis[v]) {
                dis[v] = dis[u] + v.cost;
                par[v] = u;
            }
    }
    return {std::move(dis), std::move(par)};
}
template <typename T>
std::pair<Vec<T>, vi> dijkstra(WeightedGraph<T>& g, int s = 0) {
    Vec dis(len(g), T(-1));
    vi par(len(g), -1);
    std::priority_queue<std::pair<T, int>, Vec<std::pair<T, int>>, std::greater<>> q;
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        auto [d, u] = q.top();
        q.pop();
        if (d > dis[u])
            continue;
        foreach (v, g[u])
            if (dis[v] == T(-1) || dis[u] + v.cost < dis[v]) {
                dis[v] = dis[u] + v.cost;
                par[v] = u;
                q.emplace(dis[v], v);
            }
    }
    return {std::move(dis), std::move(par)};
}
template <typename T>
std::tuple<Vec<T>, vi, vi> dijkstra(WeightedGraph<T>& g, const vi& s) {
    Vec dis(len(g), T(-1));
    vi par(len(g), -1);
    vi root(len(g), -1);
    std::priority_queue<std::pair<T, int>, Vec<std::pair<T, int>>, std::greater<>> q;
    foreach(s, s) {
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
            if (dis[v] == T(-1) || dis[u] + v.cost < dis[v]) {
                dis[v] = dis[u] + v.cost;
                par[v] = u;
                root[v] = root[u];
                q.emplace(dis[v], v);
            }
    }
    return {std::move(dis), std::move(par), std::move(root)};
}
