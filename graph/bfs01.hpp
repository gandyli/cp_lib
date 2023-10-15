#include "../graph/base.hpp"

template <typename T>
std::pair<Vec<T>, vi> bfs01(const WeightedGraph<T>& g, int s = 0) {
    Vec dis(len(g), inf<T>);
    vi par(len(g), -1);
    std::deque<int> q;
    dis[s] = 0;
    q.emplace_back(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        foreach (v, g[u])
            if (dis[u] + v.cost < dis[v]) {
                dis[v] = dis[u] + v.cost;
                par[v] = u;
                if (v.cost)
                    q.emplace_back(v);
                else
                    q.emplace_front(v);
            }
    }
    return {std::move(dis), std::move(par)};
}
template <typename T>
std::tuple<Vec<T>, vi, vi> bfs01(const WeightedGraph<T>& g, const vi& s) {
    Vec dis(len(g), inf<T>);
    vi par(len(g), -1);
    vi root(len(g), -1);
    std::deque<int> q;
    foreach(s, s) {
        dis[s] = 0;
        root[s] = s;
        q.emplace_back(s);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        foreach (v, g[u])
            if (dis[u] + v.cost < dis[v]) {
                dis[v] = dis[u] + v.cost;
                par[v] = u;
                root[v] = root[u];
                if (v.cost)
                    q.emplace_back(v);
                else
                    q.emplace_front(v);
            }
    }
    return {std::move(dis), std::move(par), std::move(root)};
}
