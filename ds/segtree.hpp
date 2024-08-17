#pragma once
#include "template.hpp"

template <typename Monoid>
struct SegTree {
    using MX = Monoid;
    using X = MX::value_type;

    int n, lg, sz;
    vc<X> a;
    SegTree() = default;
    SegTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    SegTree(const vc<T>& a) { build(a); }
    SegTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int) { return MX::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        lg = get_lg(n);
        sz = 1 << lg;
        a.assign(sz << 1, MX::unit());
        _for (i, n)
            a[i + sz] = f(i);
        _for_r (i, 1, sz)
            update(i);
    }
    void set(int i, const X& x) {
        a[i += sz] = x;
        while (i >>= 1)
            update(i);
    }
    void multiply(int i, const X& x) {
        i += sz;
        a[i] = Monoid::op(a[i], x);
        while (i >>= 1)
            update(i);
    }
    X get(int i) const { return a[i + sz]; }
    vc<X> get_all() const { return {a.begin() + sz, a.begin() + sz + n}; }
    X prod(int l, int r) const {
        X vl = MX::unit(), vr = MX::unit();
        for (l += sz, r += sz; l < r; l >>= 1, r >>= 1) {
            if (l & 1)
                vl = MX::op(vl, a[l++]);
            if (r & 1)
                vr = MX::op(a[--r], vr);
        }
        return MX::op(vl, vr);
    }
    X prod_all() const { return a[1]; }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(MX::unit()));
        if (l == n)
            return n;
        l += sz;
        X t = MX::unit();
        do {
            l >>= __builtin_ctz(l);
            if (!check(MX::op(t, a[l]))) {
                while (l < sz) {
                    l <<= 1;
                    if (check(MX::op(t, a[l])))
                        t = MX::op(t, a[l++]);
                }
                return l - sz;
            }
            t = MX::op(t, a[l++]);
        } while (l & (l - 1));
        return n;
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(MX::unit()));
        if (r == 0)
            return 0;
        r += sz - 1;
        X t = MX::unit();
        do {
            r >>= __builtin_ctz(~r);
            if (!check(MX::op(a[r], t))) {
                while (r < sz) {
                    r = r << 1 | 1;
                    if (check(MX::op(a[r], t)))
                        t = MX::op(a[r--], t);
                }
                return r + 1 - sz;
            }
            t = MX::op(a[r--], t);
        } while (r & (r + 1));
        return 0;
    }

private:
    void update(int i) { a[i] = MX::op(a[i << 1], a[i << 1 | 1]); }
};