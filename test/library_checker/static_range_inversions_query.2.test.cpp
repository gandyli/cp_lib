#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo_dual_offline.hpp"
#include "ds/sqrtfenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    Mo_Dual_Offline<Monoid_Add<i64>> mo(n);
    _for (q) {
        dR(int, l, r);
        mo.add(l, r);
    }
    auto b{a};
    UNIQUE(b);
    foreach (a, a)
        a = LB(b, a);
    int m = len(b);
    SqrtFenwickTree<Monoid_Add<int>> ft;
    auto add = [&](int i) { ft.multiply(a[i], 1); };
    auto query_l = [&](int i) { return ft.prod(a[i]); };
    auto query_r = [&](int i) { return ft.prod(a[i] + 1, m); };
    auto clear = [&] { ft.build(m); };
    io.displayArray(mo.calc(add, add, query_l, query_r, clear), '\n');
    return 0;
}