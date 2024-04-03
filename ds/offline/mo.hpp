#pragma once
#include "template.hpp"

struct Mo {
    int n;
    vc<pi> Q;
    Mo(int n = 0): n(n) {}
    void add(int l, int r) { Q.eb(l, r); }
    static vi get_order(const vc<pi>& Q, int n) {
        int m = len(Q);
        vi I(m);
        iota(all(I), 0);
        int bs = sqrt(3) * n / sqrt(2 * m);
        chkmax(bs, 1);
        sort(I, [&](int i, int j) {
            int x = Q[i].first / bs, y = Q[j].first / bs;
            return x != y ? x < y : (x & 1 ? Q[i].second > Q[j].second : Q[i].second < Q[j].second);
        });
        auto cost = [&](int i, int j) -> int {
            return abs(Q[I[i]].first - Q[I[j]].first) + abs(Q[I[i]].second - Q[I[j]].second);
        };
        _for (i, m - 5) {
            if (cost(i, i + 2) + cost(i + 1, i + 3) < cost(i, i + 1) + cost(i + 2, i + 3)) {
                swap(I[i + 1], I[i + 2]);
            }
            if (cost(i, i + 3) + cost(i + 1, i + 4) < cost(i, i + 1) + cost(i + 3, i + 4)) {
                swap(I[i + 1], I[i + 3]);
            }
        }
        return I;
    }
    void calc(auto&& add_l, auto&& add_r, auto&& del_l, auto&& del_r, auto&& query) {
        auto I = get_order(Q, n);
        int l = 0, r = 0;
        foreach (i, I) {
            auto [ql, qr] = Q[i];
            while (l > ql)
                add_l(--l);
            while (r < qr)
                add_r(r++);
            while (l < ql)
                del_l(l++);
            while (r > qr)
                del_r(--r);
            query(i);
        }
    }
};