#pragma once
#include "template.hpp"

template <typename Monoid>
struct Dual_FenwickTree {
    using MX = Monoid;
    using A = MX::value_type;

    int n;
    vc<A> a;
    Dual_FenwickTree() = default;
    Dual_FenwickTree(int n) { build(n); }

    void build(int n) {
        this->n = n;
        a.assign(n, MX::unit());
    }
    A get(int i) {
        A r = MX::unit();
        for (i++; i <= n; i += lowbit(i))
            r = MX::op(r, a[i - 1]);
        return r;
    }
    vc<A> get_all() {
        vc<A> a = this->a;
        _for_r (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[i - 1] = MX::op(a[i - 1], a[j - 1]);
        }
        return a;
    }
    void apply(int l, int r, const A& x) {
        A inv = MX::inverse(x);
        while (l < r) {
            a[r - 1] = MX::op(x, a[r - 1]);
            r -= lowbit(r);
        }
        while (r < l) {
            a[l - 1] = MX::op(inv, a[l - 1]);
            l -= lowbit(l);
        }
    }
};