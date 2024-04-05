#pragma once
#include "ds/offline/mo.hpp"

template <typename Monoid>
struct Mo_Dual_Offline {
    using M = Monoid;
    using X = M::value_type;

    int n;
    vc<pi> Q;
    Mo_Dual_Offline(int n = 0): n(n) {}
    void add(int l, int r) { Q.eb(l, r); }
    vc<X> calc(auto&& add_l, auto&& add_r, auto&& query_l, auto&& query_r, auto&& clear) {
        int m = len(Q);
        vc<X> pre(n + 1, M::unit()), suf(n + 1, M::unit());
        clear();
        _for (i, n) {
            pre[i + 1] = M::op(pre[i], query_r(i));
            add_r(i);
        }
        clear();
        _for_r (i, n) {
            suf[i] = M::op(query_l(i), suf[i + 1]);
            add_l(i);
        }
        auto Im = Mo::get_order(Q, n);
        int l = 0, r = 0;
        vvc<std::tuple<int, int, int>> QL(n + 1), QR(n + 1);
        vc<X> ans(m, M::unit());
        foreach (i, Im) {
            auto [ql, qr] = Q[i];
            ans[i] = M::op(ans[i], suf[ql]);
            ans[i] = M::op(ans[i], M::inverse(suf[l]));
            ans[i] = M::op(ans[i], pre[qr]);
            ans[i] = M::op(ans[i], M::inverse(pre[r]));
            if (l > ql)
                QR[r].eb(ql, l, ~i), l = ql;
            if (r < qr)
                QL[l].eb(r, qr, ~i), r = qr;
            if (l < ql)
                QR[r].eb(l, ql, i), l = ql;
            if (r > qr)
                QL[l].eb(qr, r, i), r = qr;
        }
        clear();
        _for (i, n) {
            foreach (l, r, j, QL[i]) {
                X s = M::unit();
                _for (k, l, r)
                    s = M::op(s, query_r(k));
                if (j >= 0)
                    ans[j] = M::op(ans[j], s);
                else
                    ans[~j] = M::op(ans[~j], M::inverse(s));
            }
            add_r(i);
        }
        clear();
        _for_r (i, n) {
            foreach (l, r, j, QR[i + 1]) {
                X s = M::unit();
                _for (k, l, r)
                    s = M::op(s, query_l(k));
                if (j >= 0)
                    ans[j] = M::op(ans[j], s);
                else
                    ans[~j] = M::op(ans[~j], M::inverse(s));
            }
            add_l(i);
        }
        _for (i, m - 1)
            ans[Im[i + 1]] = M::op(ans[Im[i + 1]], ans[Im[i]]);
        return ans;
    }
};