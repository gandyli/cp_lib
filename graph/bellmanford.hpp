#pragma once
#include "graph/base.hpp"

template <typename T>
struct BellmanFordResult {
    Vec<T> dis;
    vi par;
};
template <typename T>
auto BellmanFord(const auto& g, int s = 0) {
    int n = g.n;
    Vec dis(n, inf<T>);
    vi par(n, -1);
    dis[s] = 0;
    int cnt = 0;
    loop {
        cnt++;
        bool upd = false;
        _for (u, n)
            if (dis[u] != inf<T>)
                foreach (v, g[u]) {
                    T t = dis[u] + v.cost;
                    chkmax(t, -inf<T>);
                    if (t < dis[v]) {
                        if (cnt >= n)
                            t = -inf<T>;
                        dis[v] = t;
                        par[v] = u;
                        upd = true;
                    }
                }
        if (!upd)
            break;
    }
    return BellmanFordResult{std::move(dis), std::move(par)};
}