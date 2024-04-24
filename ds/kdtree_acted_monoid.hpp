#pragma once
#include "template.hpp"

template <typename ActedMonoid, typename T = int>
struct KDTree_ActedMonoid {
    using AM = ActedMonoid;
    using MX = ActedMonoid::Monoid_X;
    using MA = ActedMonoid::Monoid_A;
    using X = MX::value_type;
    using A = MA::value_type;
    static_assert(MX::commute());

    vc<std::tuple<T, T, T, T>> closed_range;
    vc<X> dat;
    vc<A> lazy;
    vi size;
    int n, lg;

    KDTree_ActedMonoid(vc<T> Xs, vc<T> Ys, vc<X> Vs): n(len(Xs)) {
        lg = get_lg(n);
        closed_range.resize(1 << (lg + 1));
        dat.assign(1 << (lg + 1), MX::unit());
        lazy.assign(1 << lg, MA::unit());
        size.resize(1 << (lg + 1));
        build(1, Xs, Ys, Vs);
    }
    void multiply(T x, T y, const X& v) {
        auto dfs = [&](auto&& dfs, int i) {
            auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
            if (!(xmin <= x && x <= xmax && ymin <= y && y <= ymax))
                return false;
            if (xmin == xmax && ymin == ymax) {
                dat[i] = MX::op(dat[i], v);
                size[i]++;
                return true;
            }
            push(i);
            if (dfs(dfs, i << 1) || dfs(dfs, i << 1 | 1)) {
                update(i);
                size[i] = size[i << 1] + size[i << 1 | 1];
                return true;
            }
            return false;
        };
        dfs(dfs, 1);
    }
    X prod(T xl, T xr, T yl, T yr) {
        ASSERT(xl <= xr && yl <= yr);
        auto dfs = [&](auto&& dfs, int i) {
            auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
            if (xr <= xmin || xmax < xl || yr <= ymin || ymax < yl)
                return MX::unit();
            if (xl <= xmin && xmax < xr && yl <= ymin && ymax < yr)
                return dat[i];
            push(i);
            return MX::op(dfs(dfs, i << 1), dfs(dfs, i << 1 | 1));
        };
        return dfs(dfs, 1);
    }
    X prod_all() const { return dat[1]; }
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
            update(i);
        };
        dfs(dfs, 1);
    }

private:
    void build(int i, vc<T> Xs, vc<T> Ys, vc<X> Vs, bool div_x = true) {
        int n = len(Xs);
        size[i] = n;
        auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
        xmin = ymin = inf<T>;
        xmax = ymax = -inf<T>;
        _for (i, n) {
            chkmin(xmin, Xs[i]);
            chkmax(xmax, Xs[i]);
            chkmin(ymin, Ys[i]);
            chkmax(ymax, Ys[i]);
        }
        if (xmin == xmax && ymin == ymax) {
            X x = MX::unit();
            foreach (v, Vs)
                x = MX::op(x, v);
            dat[i] = x;
            return;
        }
        int m = n >> 1;
        vi I(n);
        iota(all(I), 0);
        if (div_x)
            nth_element(I, I.begin() + m, [&](int i, int j) { return Xs[i] < Xs[j]; });
        else
            nth_element(I, I.begin() + m, [&](int i, int j) { return Ys[i] < Ys[j]; });
        Xs = rearrange(Xs, I), Ys = rearrange(Ys, I), Vs = rearrange(Vs, I);
        build(i << 1, {Xs.begin(), Xs.begin() + m}, {Ys.begin(), Ys.begin() + m}, {Vs.begin(), Vs.begin() + m}, !div_x);
        build(i << 1 | 1, {Xs.begin() + m, Xs.end()}, {Ys.begin() + m, Ys.end()}, {Vs.begin() + m, Vs.end()}, !div_x);
        update(i);
    }
    void update(int i) { dat[i] = MX::op(dat[i << 1], dat[i << 1 | 1]); }
    void apply(int i, const A& a) {
        dat[i] = AM::act(dat[i], a, size[i]);
        auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
        if (!(xmin == xmax && ymin == ymax))
            lazy[i] = MA::op(lazy[i], a);
    }
    void push(int i) {
        if (lazy[i] == MA::unit())
            return;
        apply(i << 1, lazy[i]);
        apply(i << 1 | 1, lazy[i]);
        lazy[i] = MA::unit();
    }
};