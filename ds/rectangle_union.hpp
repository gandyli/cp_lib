#pragma once
#include "ds/lazy_segtree.hpp"
#include "acted_monoid/minmincnt_add.hpp"

template <typename T = int>
struct Rectangle_Union {
    vc<T> X, Y;

    void add(T xl, T xr, T yl, T yr) {
        ASSERT(xl < xr && yl < yr);
        X.eb(xl), X.eb(xr), Y.eb(yl), Y.eb(yr);
    }
    template <typename R = i64>
    R calc() {
        int n = len(X);
        vi Ix = argsort(X), Iy = argsort(Y);
        vi rk_y(n);
        _for (i, n)
            rk_y[Iy[i]] = i;
        X = rearrange(X, Ix), Y = rearrange(Y, Iy);
        Lazy_SegTree<ActedMonoid_MinMincnt_Add<T>> seg(n - 1, [&](int i) -> std::pair<T, T> { return {0, Y[i + 1] - Y[i]}; });
        R ans = 0;
        T total = Y.back() - Y[0];
        _for (i, n - 1) {
            int k = Ix[i] >> 1;
            int a = (Ix[i] & 1 ? -1 : 1);
            seg.apply(rk_y[k << 1], rk_y[k << 1 | 1], a);
            auto [min, mincnt] = seg.prod_all();
            R dy = total - (!min ? mincnt : 0);
            R dx = X[i + 1] - X[i];
            ans += dx * dy;
        }
        return ans;
    }
};