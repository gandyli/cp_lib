#pragma once
#include "template.hpp"

template <typename Monoid>
struct FenwickTree {
    using M = Monoid;
    using X = M::value_type;

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
        a.assign(n, M::unit());
        sum = M::unit();
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        a.assign(n, M::unit());
        sum = M::unit();
        _for (i, n)
            a[i] = f(i);
        _for (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = M::op(a[i - 1], a[j - 1]);
        }
        sum = prod(n);
    }
    void set(int i, const X& x) {
        multiply(i, M::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        sum = M::op(sum, x);
        for (i++; i <= n; i += lowbit(i))
            a[i - 1] = M::op(a[i - 1], x);
    }
    X get(int i) const { return prod(i, i + 1); }
    vc<X> get_all() const {
        vc<X> a = this->a;
        _for_r (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = M::op(a[j - 1], M::inverse(a[i - 1]));
        }
        return a;
    }
    X prod(int i) const {
        X r = M::unit();
        while (i) {
            r = M::op(r, a[i - 1]);
            i -= lowbit(i);
        }
        return r;
    }
    X prod(int l, int r) const {
        X vl = M::unit(), vr = M::unit();
        while (l < r) {
            vr = M::op(vr, a[r - 1]);
            r -= lowbit(r);
        }
        while (r < l) {
            vl = M::op(vl, a[l - 1]);
            l -= lowbit(l);
        }
        return M::op(vr, M::inverse(vl));
    }
    X prod_all() const { return sum; }
    int max_right(auto&& check) const {
        ASSERT(check(M::unit()));
        int i = 0;
        X t = M::unit();
        int k = 1;
        while ((k << 1) <= n)
            k <<= 1;
        while (k) {
            if (i + k <= n) {
                int nt = M::op(t, a[i + k - 1]);
                if (check(nt)) {
                    t = nt;
                    i += k;
                }
            }
            k >>= 1;
        }
        return i;
    }
    int kth(int x) {
        return max_right([&](const X& y) { return y <= x; });
    }
};