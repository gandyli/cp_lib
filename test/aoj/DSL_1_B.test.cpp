#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_B"

#include "all.hpp"
#include "ds/weighted_unionfind.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    Weighted_UnionFind<Monoid_Add<int>> wuf(n);
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            dR(int, z);
            wuf.merge(x, y, z);
        }
        else {
            auto [x0, vx] = wuf[x];
            auto [y0, vy] = wuf[y];
            if (x0 != y0)
                print('?');
            else
                print(vy - vx);
        }
    }
    return 0;
}