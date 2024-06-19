#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#include "all.hpp"
#include "graph/centroid_decomposition.hpp"
#include "poly/ntt_avx2.hpp"
#include "poly/convolution_square.hpp"

int main() {
    dR(int, n);
    auto g = read_tree(n, 0);
    vc<i64> ans(n);
    auto f = [&](auto&& par, auto&& V, auto&& indptr) {
        int n = len(V);
        vi dist(n);
        _for (i, 1, n)
            dist[i] = dist[par[i]] + 1;
        auto calc = [&](int l, int r, int sgn) {
            subrange D{dist.begin() + l, dist.begin() + r};
            int mx = max(D);
            vc<i64> f(mx + 1);
            foreach (d, D)
                f[d]++;
            f = convolution_square(f);
            _for (i, min(len(f), len(ans)))
                ans[i] += sgn * f[i];
        };
        calc(0, n, 1);
        _for (i, 1, len(indptr) - 1)
            calc(indptr[i], indptr[i + 1], -1);
    };
    centroid_decomposition<0>(g, f);
    ans.erase(ans.begin());
    foreach (x, ans)
        x >>= 1;
    print(ans);
    return 0;
}