#pragma once
#include "template.hpp"

template <typename Monoid>
struct Weighted_UnionFind {
    using MX = Monoid;
    using X = MX::value_type;

    int n, comp;
    vi a;
    vc<X> w;
    Weighted_UnionFind(int n = 0) { build(n); }
    void build(int n) {
        this->n = comp = n;
        a.assign(n, -1);
        w.assign(n, MX::unit());
    }
    std::pair<int, X> get(int x) {
        X r = MX::unit();
        while (a[x] >= 0) {
            r = MX::op(r, w[x]);
            r = MX::op(r, w[a[x]]);
            w[x] = MX::op(w[a[x]], w[x]);
            if (a[a[x]] < 0)
                return {a[x], r};
            x = a[x] = a[a[x]];
        }
        return {x, r};
    }
    std::pair<int, X> operator[](int x) { return get(x); }
    bool same(int x, int y) { return get(x).first == get(y).first; }
    bool merge(int x, int y, X z) {
        auto [x0, vx] = get(x);
        auto [y0, vy] = get(y);
        if (x0 == y0)
            return false;
        comp--;
        if (a[x0] > a[y0]) {
            swap(x0, y0);
            swap(vx, vy);
            z = MX::inverse(z);
        }
        z = MX::op(MX::op(vx, z), MX::inverse(vy));
        a[x0] += a[y0];
        a[y0] = x0;
        w[y0] = z;
        return true;
    }
};