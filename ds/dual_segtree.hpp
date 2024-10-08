#pragma once
#include "template.hpp"

template <typename Monoid>
struct Dual_SegTree {
    using MX = Monoid;
    using A = MX::value_type;

    int n, lg, sz;
    vc<A> a;
    Dual_SegTree() = default;
    Dual_SegTree(int n) { build(n); }

    void build(int n) {
        this->n = n;
        lg = get_lg(n);
        sz = 1 << lg;
        a.assign(sz << 1, MX::unit());
    }
    A get(int i) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        return a[i];
    }
    vc<A> get_all() {
        _for (i, sz)
            push(i);
        return {a.begin() + sz, a.begin() + sz + n};
    }
    void apply(int l, int r, const A& x) {
        if (l == r)
            return;
        l += sz, r += sz;
        if constexpr (!MX::commute())
            _for_r (i, 1, lg + 1) {
                if (((l >> i) << i) != l)
                    push(l >> i);
                if (((r >> i) << i) != r)
                    push((r - 1) >> i);
            }
        while (l < r) {
            if (l & 1)
                apply(l++, x);
            if (r & 1)
                apply(--r, x);
            l >>= 1, r >>= 1;
        }
    }

private:
    void apply(int i, const A& x) { a[i] = MX::op(a[i], x); }
    void push(int i) {
        if (a[i] == MX::unit())
            return;
        apply(i << 1, a[i]);
        apply(i << 1 | 1, a[i]);
        a[i] = MX::unit();
    }
};