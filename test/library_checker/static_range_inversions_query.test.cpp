#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo.hpp"
#include "ds/fenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    auto b{a};
    UNIQUE(b);
    foreach (a, a)
        a = LB(b, a);
    int m = len(b);
    Mo mo(n);
    _for (q) {
        dR(int, l, r);
        mo.add(l, r);
    }
    i64 Ans = 0;
    vc<i64> ans(q);
    FenwickTree<Monoid_Add<int>> ft(m);
    auto add_l = [&](int i) {
        Ans += ft.prod(a[i]);
        ft.multiply(a[i], 1);
    };
    auto add_r = [&](int i) {
        Ans += ft.prod(a[i] + 1, m);
        ft.multiply(a[i], 1);
    };
    auto del_l = [&](int i) {
        ft.multiply(a[i], -1);
        Ans -= ft.prod(a[i]);
    };
    auto del_r = [&](int i) {
        ft.multiply(a[i], -1);
        Ans -= ft.prod(a[i] + 1, m);
    };
    auto query = [&](int i) {
        ans[i] = Ans;
    };
    mo.calc(add_l, add_r, del_l, del_r, query);
    io.displayArray(ans, '\n');
    return 0;
}