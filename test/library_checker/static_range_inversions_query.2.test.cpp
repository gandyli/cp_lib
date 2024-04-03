#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo.hpp"
#include "ds/fenwicktree.hpp"
#include "ds/sqrtfenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    dRV(pi, Q, q);
    auto b{a};
    UNIQUE(b);
    foreach (a, a)
        a = LB(b, a);
    int m = len(b);
    FenwickTree<Monoid_Add<int>> ft(m);
    vc<i64> pre(n + 1), suf(n + 1);
    _for (i, n) {
        pre[i + 1] = pre[i] + ft.prod(a[i] + 1, m);
        ft.multiply(a[i], 1);
    }
    ft.build(m);
    _for_r (i, n) {
        suf[i] = suf[i + 1] + ft.prod(a[i]);
        ft.multiply(a[i], 1);
    }
    auto Im = Mo::get_order(Q, n);
    int l = 0, r = 0;
    vvc<std::tuple<int, int, int>> QL(n + 1), QR(n + 1);
    vc<i64> ans(q);
    foreach (i, Im) {
        auto [ql, qr] = Q[i];
        if (l > ql) {
            QR[r].eb(ql, l, ~i);
            ans[i] += suf[ql] - suf[l];
            l = ql;
        }
        if (r < qr) {
            QL[l].eb(r, qr, ~i);
            ans[i] += pre[qr] - pre[r];
            r = qr;
        }
        if (l < ql) {
            QR[r].eb(l, ql, i);
            ans[i] += suf[ql] - suf[l];
            l = ql;
        }
        if (r > qr) {
            QL[l].eb(qr, r, i);
            ans[i] += pre[qr] - pre[r];
            r = qr;
        }
    }
    SqrtFenwickTree<Monoid_Add<int>> sft(m);
    _for (i, n) {
        foreach (l, r, j, QL[i]) {
            i64 s = 0;
            _for (k, l, r)
                s += sft.prod(a[k] + 1, m);
            if (j >= 0)
                ans[j] += s;
            else
                ans[~j] -= s;
        }
        sft.multiply(a[i], 1);
    }
    sft.build(m);
    _for_r (i, n) {
        foreach (l, r, j, QR[i + 1]) {
            i64 s = 0;
            _for (k, l, r)
                s += sft.prod(a[k]);
            if (j >= 0)
                ans[j] += s;
            else
                ans[~j] -= s;
        }
        sft.multiply(a[i], 1);
    }
    _for (i, q - 1)
        ans[Im[i + 1]] += ans[Im[i]];
    io.displayArray(ans, '\n');
    return 0;
}