#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include "all.hpp"
#include "ds/offline/mo.hpp"
#include "ds/fenwicktree_01.hpp"
#include "ds/sqrtfenwicktree_01.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    dRV(pi, Q, q);
    auto I = argsort(a);
    vi b(n);
    _for (i, n)
        b[I[i]] = i;
    a = std::move(b);
    FenwickTree_01 ft(n);
    vc<i64> pre(n + 1), suf(n + 1);
    _for (i, n) {
        pre[i + 1] = pre[i] + ft.prod(a[i], n);
        ft.add(a[i]);
    }
    ft.build(n);
    _for_r (i, n) {
        suf[i] = suf[i + 1] + ft.prod(a[i]);
        ft.add(a[i]);
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
    SqrtFenwickTree_01 sft(n);
    _for (i, n) {
        foreach (l, r, j, QL[i]) {
            i64 s = 0;
            _for (k, l, r)
                s += sft.prod(a[k], n);
            if (j >= 0)
                ans[j] += s;
            else
                ans[~j] -= s;
        }
        sft.add(a[i]);
    }
    sft.build(n);
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
        sft.add(a[i]);
    }
    _for (i, q - 1)
        ans[Im[i + 1]] += ans[Im[i]];
    io.displayArray(ans, '\n');
    return 0;
}