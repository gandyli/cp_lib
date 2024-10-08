#pragma once
#include "graph/base.hpp"

template <typename T>
struct BellmanFordResult {
    vc<T> dis;
    vi par;
};
template <typename T>
auto BellmanFord(const auto& g, int s = 0) {
    const int n = g.n;
    vc dis(n, inf<T>);
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
                    if (dis[u] == -inf<T>)
                        t = -inf<T>;
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