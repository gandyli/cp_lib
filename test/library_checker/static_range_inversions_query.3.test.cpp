#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo_dual_offline.hpp"
#include "ds/sqrtfenwicktree_01.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    Mo_Dual_Offline<Monoid_Add<i64>> mo(n);
    _for (q) {
        dR(int, l, r);
        mo.add(l, r);
    }
    auto I = argsort(a);
    vi b(n);
    _for (i, n)
        b[I[i]] = i;
    a = std::move(b);
    SqrtFenwickTree<Monoid_Add<int>> ft;
    auto add = [&](int i) { ft.multiply(a[i], 1); };
    auto query_l = [&](int i) { return ft.prod(a[i]); };
    auto query_r = [&](int i) { return ft.prod(a[i], n); };
    auto clear = [&] { ft.build(n); };
    io.displayArray(mo.calc(add, add, query_l, query_r, clear), '\n');
    return 0;
}