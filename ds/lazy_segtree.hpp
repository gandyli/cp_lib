#pragma once
#include "template.hpp"

template <typename ActedMonoid, bool beats = false>
struct Lazy_SegTree_Base {
    using AM = ActedMonoid;
    using MX = ActedMonoid::Monoid_X;
    using MA = ActedMonoid::Monoid_A;
    using X = MX::value_type;
    using A = MA::value_type;
    int n, lg, sz;
    Vec<X> a;
    Vec<A> lazy;

    Lazy_SegTree_Base() = default;
    Lazy_SegTree_Base(int n) { build(n); }
    Lazy_SegTree_Base(const Vec<X>& a) { build(a); }
    Lazy_SegTree_Base(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return MX::unit(); });
    }
    void build(const Vec<X>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        lg = 1;
        while ((1 << lg) < n)
            ++lg;
        sz = 1 << lg;
        a.assign(sz << 1, MX::unit());
        lazy.assign(sz, MA::unit());
        _for (i, n)
            a[sz + i] = f(i);
        _for_r (i, 1, sz)
            update(i);
    }
    void update(int i) { a[i] = MX::op(a[i << 1], a[i << 1 | 1]); }
    void set(int i, const X& x) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        a[i] = x;
        _for (j, 1, lg + 1)
            update(i >> j);
    }
    void multiply(int i, const X& x) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        a[i] = MX::op(a[i], x);
        _for (j, 1, lg + 1)
            update(i >> j);
    }
    X get(int i) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        return a[i];
    }
    Vec<X> get_all() {
        _for (i, sz)
            push(i);
        return {a.begin() + sz, a.begin() + sz + n};
    }
    X prod(int l, int r) {
        if (l == r)
            return MX::unit();
        l += sz, r += sz;
        _for_r (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        X xl = MX::unit(), xr = MX::unit();
        while (l < r) {
            if (l & 1)
                xl = MX::op(xl, a[l++]);
            if (r & 1)
                xr = MX::op(a[--r], xr);
            l >>= 1, r >>= 1;
        }
        return MX::op(xl, xr);
    }
    X prod_all() { return a[1]; }
    void apply(int l, int r, const A& x) {
        if (l == r)
            return;
        l += sz, r += sz;
        _for_r (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1)
                apply(l++, x);
            if (r & 1)
                apply(--r, x);
            l >>= 1, r >>= 1;
        }
        l = l2, r = r2;
        _for (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                update(l >> i);
            if (((r >> i) << i) != r)
                update((r - 1) >> i);
        }
    }
    int max_right(auto&& check, int l) {
        ASSERT(0 <= l && l <= n && check(MX::unit()));
        if (l == n)
            return n;
        l += sz;
        _for_r (i, 1, lg + 1)
            push(l >> i);
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
    int min_left(auto&& check, int r) {
        ASSERT(0 <= r && r <= n && check(MX::unit()));
        if (r == 0)
            return 0;
        r += sz;
        _for_r (i, 1, lg + 1)
            push((r - 1) >> i);
        X t = MX::unit();
        do {
            r--;
            r = (r & (r + 1)) == 0 ? 1 : r >> __builtin_ctz(~r);
            if (!check(MX::op(a[r], t))) {
                while (r < sz) {
                    r = r << 1 | 1;
                    if (check(MX::op(a[r], t)))
                        t = MX::op(a[r--], t);
                }
                return r + 1 - sz;
            }
            t = MX::op(a[r], t);
        } while (r & (r - 1));
        return 0;
    }

private:
    void apply(int i, const A& x) {
        a[i] = AM::act(a[i], x, 1 << (lg - std::__lg(i)));
        if (i < sz) {
            lazy[i] = MA::op(lazy[i], x);
            if constexpr (beats)
                if (a[i].fail)
                    push(i), update(i);
        }
    }
    void push(int i) {
        if (lazy[i] == MA::unit())
            return;
        apply(i << 1, lazy[i]);
        apply(i << 1 | 1, lazy[i]);
        lazy[i] = MA::unit();
    }
};
template <typename ActedMonoid>
using Lazy_SegTree = Lazy_SegTree_Base<ActedMonoid>;