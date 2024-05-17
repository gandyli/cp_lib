#pragma once
#include "graph/base.hpp"

template <typename T>
std::pair<T, vi> minimum_hamiltonian_cycle_dis(const vvc<T>& dis) {
    int n = len(dis) - 1;
    VECI(dp, 1 << n, n, inf<T>);
    _for (i, n)
        chkmin(dp[1 << i][i], dis[n][i]);
    _for (s, 1 << n) {
        _for (u, n) {
            if (dp[s][u] >= inf<T>)
                continue;
            _for (v, n) {
                if (s >> v & 1)
                    continue;
                int t = s | 1 << v;
                if (dis[u][v] < inf<T>)
                    chkmin(dp[t][v], dp[s][u] + dis[u][v]);
            }
        }
    }
    int s = (1 << n) - 1;
    T ans = inf<T>;
    int best = -1;
    _for (i, n)
        if (dis[i][n] < inf<T> && dp[s][i] < inf<T>)
            if (chkmin(ans, dp[s][i] + dis[i][n]))
                best = i;
    if (best == -1)
        return {inf<T>, {}};
    vi path = {n, best};
    int t = s;
    while (len(path) <= n) {
        int v = path.back();
        _for (u, n) {
            int s = t ^ (1 << v);
            if (dis[u][v] < inf<T> && dp[s][u] < inf<T> && dp[t][v] == dp[s][u] + dis[u][v]) {
                path.eb(u);
                t = s;
                break;
            }
        }
    }
    reverse(path);
    return {ans, std::move(path)};
}
template <typename T>
std::pair<T, vi> minimum_hamiltonian_cycle(const auto& g) {
    const int n = g.n;
    VECI(dis, n, n, inf<T>);
    _for (u, n)
        foreach (v, g[u])
            chkmin(dis[u][v], v.cost);
    return minimum_hamiltonian_cycle_dis(dis);
}