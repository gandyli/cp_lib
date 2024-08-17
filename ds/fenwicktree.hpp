#pragma once
#include "template.hpp"

template <typename Monoid>
struct FenwickTree {
    using MX = Monoid;
    using X = MX::value_type;

    int n;
    vc<X> a;
    X sum;
    FenwickTree() = default;
    FenwickTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    FenwickTree(const vc<T>& a) { build(a); }
    FenwickTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        a.assign(n, MX::unit());
        sum = MX::unit();
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        a.assign(n, MX::unit());
        _for (i, n)
            a[i] = f(i);
        _for (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = MX::op(a[i - 1], a[j - 1]);
        }
        sum = prod(n);
    }
    void set(int i, const X& x) {
        multiply(i, MX::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        sum = MX::op(sum, x);
        for (i++; i <= n; i += lowbit(i))
            a[i - 1] = MX::op(a[i - 1], x);
    }
    X get(int i) const { return prod(i, i + 1); }
    vc<X> get_all() const {
        vc<X> a = this->a;
        _for_r (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = MX::op(a[j - 1], MX::inverse(a[i - 1]));
        }
        return a;
    }
    X prod(int i) const {
        X r = MX::unit();
        while (i) {
            r = MX::op(r, a[i - 1]);
            i -= lowbit(i);
        }
        return r;
    }
    X prod(int l, int r) const {
        X vl = MX::unit(), vr = MX::unit();
        while (l < r) {
            vr = MX::op(vr, a[r - 1]);
            r -= lowbit(r);
        }
        while (r < l) {
            vl = MX::op(vl, a[l - 1]);
            l -= lowbit(l);
        }
        return MX::op(vr, MX::inverse(vl));
    }
    X prod_all() const { return sum; }
    int max_right(auto&& check, int l = 0) const {
        ASSERT(check(MX::unit()));
        int i = l;
        X t = MX::unit();
        int k = BLK {
            loop {
                if (i & 1)
                    t = MX::op(t, MX::inverse(a[--i]));
                if (i == 0)
                    return std::__lg(n) + 1;
                int k = __builtin_ctz(i) - 1;
                if (i + (1 << k) > n)
                    return k;
                if (!check(MX::op(t, a[i + (1 << k) - 1])))
                    return k;
                t = MX::op(t, MX::inverse(a[i - 1]));
                i -= lowbit(i);
            }
        };
        while (k--) {
            if (i + (1 << k) <= n) {
                X nt = MX::op(t, a[i + (1 << k) - 1]);
                if (check(nt)) {
                    t = nt;
                    i += 1 << k;
                }
            }
        }
        return i;
    }
    int min_left(auto&& check, int r) const {
        ASSERT(check(MX::unit()));
        int i = r;
        int k = 0;
        X t = MX::unit();
        while (i && check(t)) {
            t = MX::op(t, a[i - 1]);
            k = __builtin_ctz(i);
            i -= lowbit(i);
        }
        if (i == 0)
            return 0;
        while (k--) {
            X nt = MX::op(t, MX::inverse(a[i + (1 << k) - 1]));
            if (!check(nt)) {
                t = nt;
                i += 1 << k;
            }
        }
        return i + 1;
    }
    int kth(int x, int l = 0) const {
        return max_right([&](const X& y) { return y <= x; }, l);
    }
};