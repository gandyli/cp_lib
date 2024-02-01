#define PROBLEM "https://atcoder.jp/contests/abc324/tasks/abc324_f"

#include "all.hpp"
#include "graph/base.hpp"
#include "math/binary_search.hpp"

int main() {
    dR(int, n, m);
    Graph<void, true> g(n, m);
    vi B(m), C(m);
    _for (i, m) {
        dR(int, u, v).read(B[i], C[i]), u--, v--;
        g.add(u, v);
    }
    g.build();
    auto check = [&](auto mi) {
        vc dp(n, -inf<ld>);
        dp[0] = 0;
        _for (u, n)
            foreach (v, g[u])
                chkmax(dp[v], dp[u] + B[v.id] - mi * C[v.id]);
        return dp[n - 1] >= 0;
    };
    io.setprec(16);
    print(bsearch<ld>(check, 0, inf<int>));
    return 0;
}