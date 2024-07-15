#pragma once
#include "graph/base.hpp"

pi find_centroids(const auto& g) {
    const int n = g.n;
    vi par(n, -1), V(n), sz(n);
    int l = 0, r = 0;
    V[r++] = 0;
    while (l < r) {
        int u = V[l++];
        foreach (v, g[u])
            if (v != par[u]) {
                V[r++] = v;
                par[v] = u;
            }
    }
    _for_r (i, n) {
        int u = V[i], p = par[u];
        sz[u]++;
        if (p != -1)
            sz[p] += sz[u];
    }
    int m = n / 2;
    pi ans{-1, -1};
    _for (u, n) {
        if (BLK {
                if (n - sz[u] > m)
                    return false;
                foreach (v, g[u])
                    if (v != par[u] && sz[v] > m)
                        return false;
                return true;
            }) {
            if (ans.first == -1)
                ans.first = u;
            else
                ans.second = u;
        }
    }
    return ans;
}