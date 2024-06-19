#pragma once
#include "graph/centroid_decomposition.hpp"
#include "poly/convolution.hpp"

vc<i64> tree_all_distances(const auto& g) {
    const int n = g.n;
    vc<i64> ans(n);
    ans[0] = n;
    ans[1] = n + n - 2;
    auto f = [&](auto&& par, auto&& V, int n1, int n2) {
        int n = len(V);
        vi dist(n);
        _for (i, 1, n)
            dist[i] = dist[par[i]] + 1;
        int mx = max(dist);
        vi f(mx + 1), g(mx + 1);
        _for (i, 1, n1 + 1)
            f[dist[i]]++;
        _for (i, n1 + 1, n1 + n2 + 1)
            g[dist[i]]++;
        while (f.back() == 0)
            f.pop_back();
        while (g.back() == 0)
            g.pop_back();
        auto r = convolution<int, i64>(f, g);
        _for (i, len(r))
            ans[i] += r[i] * 2;
    };
    centroid_decomposition<1>(g, f);
    return ans;
}