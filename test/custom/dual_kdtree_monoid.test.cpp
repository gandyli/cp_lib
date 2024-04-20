#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "ds/dual_kdtree_monoid.hpp"
#include "monoid/add.hpp"
#include "random/base.hpp"

void test() {
    _for (100) {
        int M = rnd(1, 100), N = rnd(1, 100);
        using Mono = Monoid_Add<int>;
        int dat[100][100]{};
        vi X, Y;
        std::set<pi> S;
        _for (i, N) {
            int x = rnd(0, M), y = rnd(0, M);
            if (S.emplace(x, y).second)
                X.eb(x), Y.eb(y);
        }
        N = len(X);
        Dual_KDTree_Monoid<Mono> tree(X, Y);
        int Q = 100;
        _for (Q) {
            int t = rnd(0, 2);
            int xl = rnd(0, M), xr = rnd(0, M), yl = rnd(0, M), yr = rnd(0, M);
            if (xl > xr)
                swap(xl, xr);
            if (yl > yr)
                swap(yl, yr);
            if (t == 0) {
                int v = rnd(0, 100);
                _for (i, xl, xr)
                    _for (j, yl, yr)
                        dat[i][j] += v;
                tree.apply(xl, xr, yl, yr, v);
            }
            if (t == 1) {
                int k = rnd(0, N);
                int x = X[k], y = Y[k];
                auto t = tree.get(k);
                assert(t == dat[x][y]);
            }
        }
    }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}