#define PROBLEM "https://atcoder.jp/contests/past202005-open/tasks/past202005_m"

#include "all.hpp"
#include "graph/bfs01.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph(n, m);
    dR(int, s, K), s--;
    dRV(int, a, K);
    foreach (a, a)
        a--;
    vvi d_a(K);
    _for (i, K)
        d_a[i] = bfs01<int>(g, a[i]).dis;
    auto d_s = bfs01<int>(g, s).dis;
    VECI(dp, 1 << K, K, inf<int>);
    _for (i, K)
        dp[1 << i][i] = d_s[a[i]];
    _for (s, 1 << K) {
        _for (i, K) {
            if (dp[s][i] == inf<int>)
                continue;
            _for (j, K)
                if (!(s >> j & 1))
                    chkmin(dp[s | 1 << j][j], dp[s][i] + d_a[i][a[j]]);
        }
    }
    print(min(dp.back()));
    return 0;
}