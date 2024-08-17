#pragma once
#include "math/binary_search.hpp"
#include "ds/sparse_table.hpp"

template <typename Monoid, int LG = 4>
struct Static_RMQ {
    using MX = Monoid;
    using X = MX::value_type;

    int n;
    vc<X> a, pre, suf;
    Sparse_Table<MX> st;
    vc<u16> dat;
    Static_RMQ() = default;
    Static_RMQ(int n) { build(n); }
    template <std::convertible_to<X> T>
    Static_RMQ(const vc<T>& a) { build(a); }
    Static_RMQ(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return MX::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        a.resize(n);
        _for (i, n)
            a[i] = f(i);
        pre = suf = a;
        constexpr int mask = (1 << LG) - 1;
        _for (i, 1, n)
            if (i & mask)
                pre[i] = MX::op(pre[i - 1], a[i]);
        _for_r (i, 1, n)
            if (i & mask)
                suf[i - 1] = MX::op(a[i - 1], suf[i]);
        st.build(n >> LG, [&](int i) { return suf[i << LG]; });
        dat.resize(n);
        u32 st = 0;
        _for_r (i, n) {
            st = (st << 1) & 65535;
            while (st) {
                int k = __builtin_ctz(st);
                if (MX::op(a[i], a[i + k]) != a[i])
                    break;
                st &= ~(1U << k);
            }
            st |= 1;
            dat[i] = st;
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return MX::unit();
        if (r - l <= 16) {
            u32 d = dat[l] & ((1U << (r - l)) - 1);
            return a[l + std::__lg(d)];
        }
        r--;
        int x = l >> LG, y = r >> LG;
        return MX::op(MX::op(suf[l], st.prod(x + 1, y)), pre[r]);
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(MX::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(MX::unit()));
        if (r == 0)
            return 0;
        return bsearch([&](int l) { return check(prod(l, r)); }, r, -1);
    }
};