#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#include "all.hpp"
#include "graph/centroid_decomposition.hpp"
#include "poly/ntt_avx2.hpp"
#include "poly/convolution.hpp"

int main() {
    dR(int, n);
    auto g = read_tree(n, 0);
    vc<i64> ans(n);
    auto f = [&](auto&& par, auto&& V, auto&& col) {
        int n = len(V);
        vi dist(n);
        _for (i, 1, n)
            dist[i] = dist[par[i]] + 1;
        int mx = max(dist);
        vi f(mx + 1), g(mx + 1);
        _for (i, n) {
            if (col[i] == 0)
                f[dist[i]]++;
            if (col[i] == 1)
                g[dist[i]]++;
        }
        while (!f.empty() && f.back() == 0)
            f.pop_back();
        while (!g.empty() && g.back() == 0)
            g.pop_back();
        auto r = convolution<int, i64>(f, g);
        _for (i, len(r))
            ans[i] += r[i];
    };
    centroid_decomposition<2>(g, f);
    ans.erase(ans.begin());
    print(ans);
    return 0;
}