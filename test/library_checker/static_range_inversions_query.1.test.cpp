#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo.hpp"
#include "ds/fenwicktree_01.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    auto I = argsort(a);
    vi b(n);
    _for (i, n)
        b[I[i]] = i;
    a = std::move(b);
    Mo mo(n);
    _for (q) {
        dR(int, l, r);
        mo.add(l, r);
    }
    i64 Ans = 0;
    vc<i64> ans(q);
    FenwickTree_01 ft(n);
    auto add_l = [&](int i) {
        Ans += ft.prod(a[i]);
        ft.add(a[i]);
    };
    auto add_r = [&](int i) {
        Ans += ft.prod(a[i] + 1, n);
        ft.add(a[i]);
    };
    auto del_l = [&](int i) {
        ft.remove(a[i]);
        Ans -= ft.prod(a[i]);
    };
    auto del_r = [&](int i) {
        ft.remove(a[i]);
        Ans -= ft.prod(a[i] + 1, n);
    };
    auto query = [&](int i) {
        ans[i] = Ans;
    };
    mo.calc(add_l, add_r, del_l, del_r, query);
    io.displayArray(ans, '\n');
    return 0;
}