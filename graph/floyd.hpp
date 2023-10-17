#pragma once
#include "base.hpp"

template <typename T>
Vec<Vec<T>> floyd(const auto& g) {
    int n = g.n;
    VECI(dis, n, n, inf<T>);
    _for (i, n)
        dis[i][i] = 0;
    _for (u, n)
        foreach (v, g[u])
            chkmin(dis[u][v], v.cost);
    _for (k, n)
        _for (i, n)
            _for (j, n)
                chkmin(dis[i][j], dis[i][k] + dis[k][j]);
    return dis;
}