#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "ds/kdtree_acted_monoid.hpp"
#include "acted_monoid/summax_add.hpp"
#include "random/base.hpp"

void test() {
    _for (100) {
        int M = rnd(1, 100), N = rnd(1, 100);
        using AM = ActedMonoid_SumMax_Add<int>;
        using Mono = AM::Monoid_X;
        
        vi dat[100][100];
        vi X, Y;
        vc<Mono::value_type> V;
        _for (i, N) {
            int x = rnd(0, M), y = rnd(0, M), v = rnd(0, 100);
            dat[x][y].eb(v);
            X.eb(x), Y.eb(y), V.eb(v, v);
        }
        KDTree_ActedMonoid<AM> tree(X, Y, V);
        int Q = 100;
        _for (Q) {
            int t = rnd(0, 4);
            int xl = rnd(0, M), xr = rnd(0, M), yl = rnd(0, M), yr = rnd(0, M);
            if (xl > xr)
                swap(xl, xr);
            if (yl > yr)
                swap(yl, yr);
            if (t == 0) {
                int k = rnd(0, N);
                int x = X[k], y = Y[k];
                int v = rnd(0, 100);
                dat[x][y].eb(v);
                tree.multiply(x, y, {v, v});
            }
            if (t == 1) {
                int sm = 0, mx = -inf<int>;
                _for (i, xl, xr)
                    _for (j, yl, yr)
                        foreach (x, dat[i][j])
                            sm += x, chkmax(mx, x);
                auto [a, b] = tree.prod(xl, xr, yl, yr);
                assert(a == sm && b == mx);
            }
            if (t == 2) {
                int sm = 0, mx = -inf<int>;
                _for (i, M)
                    _for (j, M)
                        foreach (x, dat[i][j])
                            sm += x, chkmax(mx, x);
                auto [a, b] = tree.prod_all();
                assert(a == sm && b == mx);
            }
            if (t == 3) {
                int v = rnd(0, 100);
                _for (i, xl, xr)
                    _for (j, yl, yr)
                        foreach (x, dat[i][j])
                            x += v;
                tree.apply(xl, xr, yl, yr, v);
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