#pragma once
#include "graph/base.hpp"

template <typename T>
auto floyd(const auto& g) {
    int n = g.n;
    VECI(dis, n, n, inf<T>);
    _for (i, n)
        dis[i][i] = 0;
    _for (u, n)
        foreach (v, g[u])
            chkmin(dis[u][v], v.cost);
    _for (k, n)
        _for (i, n)
            if (dis[i][k] != inf<T>)
                _for (j, n)
                    if (dis[k][j] != inf<T>)
                        chkmin(dis[i][j], dis[i][k] + dis[k][j]);
    return dis;
}