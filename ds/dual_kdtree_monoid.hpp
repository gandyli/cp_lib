#pragma once
#include "template.hpp"

template <typename Monoid, typename T = int>
struct Dual_KDTree_Monoid {
    using M = Monoid;
    using A = M::value_type;

    vc<std::tuple<T, T, T, T>> closed_range;
    vc<A> lazy;
    vi where;
    int n, lg;

    Dual_KDTree_Monoid(const vc<T>& X, const vc<T>& Y): n(len(X)) {
        lg = get_lg(n);
        closed_range.resize(1 << (lg + 1));
        lazy.assign(1 << (lg + 1), M::unit());
        where.resize(n);
        vi I(n);
        iota(all(I), 0);
        build(1, X, Y, I);
    }
    void apply(T xl, T xr, T yl, T yr, const A& a) {
        ASSERT(xl <= xr && yl <= yr);
        auto dfs = [&](auto&& dfs, int i) {
            auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
            if (xr <= xmin || xmax < xl || yr <= ymin || ymax < yl)
                return;
            if (xl <= xmin && xmax < xr && yl <= ymin && ymax < yr)
                return apply(i, a);
            push(i);
            dfs(dfs, i << 1);
            dfs(dfs, i << 1 | 1);
        };
        dfs(dfs, 1);
    }
    A get(int i) {
        T x = where[i];
        _for_r (j, 1, lg + 1)
            push(x >> j);
        return lazy[x];
    }

private:
    void build(int i, vi X, vi Y, vi raw_idx, bool div_x = true) {
        int n = len(X);
        auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
        xmin = ymin = inf<T>;
        xmax = ymax = -inf<T>;
        _for (i, n) {
            chkmin(xmin, X[i]);
            chkmax(xmax, X[i]);
            chkmin(ymin, Y[i]);
            chkmax(ymax, Y[i]);
        }
        if (xmin == xmax && ymin == ymax) {
            ASSERT(len(raw_idx) == 1);
            where[raw_idx[0]] = i;
            return;
        }
        int m = n >> 1;
        vi I(n);
        iota(all(I), 0);
        if (div_x)
            nth_element(I, I.begin() + m, [&](int i, int j) { return X[i] < X[j]; });
        else
            nth_element(I, I.begin() + m, [&](int i, int j) { return Y[i] < Y[j]; });
        X = rearrange(X, I), Y = rearrange(Y, I), raw_idx = rearrange(raw_idx, I);
        build(i << 1, {X.begin(), X.begin() + m}, {Y.begin(), Y.begin() + m}, {raw_idx.begin(), raw_idx.begin() + m}, !div_x);
        build(i << 1 | 1, {X.begin() + m, X.end()}, {Y.begin() + m, Y.end()}, {raw_idx.begin() + m, raw_idx.end()}, !div_x);
    }
    void apply(int i, const A& a) { lazy[i] = M::op(lazy[i], a); }
    void push(int i) {
        if (lazy[i] == M::unit())
            return;
        apply(i << 1, lazy[i]);
        apply(i << 1 | 1, lazy[i]);
        lazy[i] = M::unit();
    }
};