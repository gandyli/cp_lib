#pragma once
#include "../graph/base.hpp"

template <typename T>
std::pair<Vec<T>, vi> BellmanFord(const WeightedGraph<T>& g, int s = 0) {
    Vec dis(len(g), inf<T>);
    vi par(len(g), -1);
    dis[s] = 0;
    int lop = 0;
    loop {
        lop++;
        bool upd = false;
        _for (u, len(g))
            if (dis[u] != inf<T>)
                foreach (v, g[u]) {
                    T t = dis[u] + v.cost;
                    chkmax(t, -inf<T>);
                    if (t < dis[v]) {
                        if (lop >= len(g))
                            t = -inf<T>;
                        dis[v] = t;
                        par[v] = u;
                        upd = true;
                    }
                }
        if (!upd)
            break;
    }
    return {std::move(dis), std::move(par)};
}