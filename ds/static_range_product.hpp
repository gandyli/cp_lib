#pragma once
#include "math/binary_search.hpp"

template <template <typename> typename ST, typename Monoid, int LG = 4>
struct Static_Range_Product {
    using M = Monoid;
    using X = M::value_type;

    int n;
    vc<X> a, pre, suf;
    ST<M> st;
    Static_Range_Product() = default;
    Static_Range_Product(int n) { build(n); }
    template <std::convertible_to<X> T>
    Static_Range_Product(const vc<T>& a) { build(a); }
    Static_Range_Product(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return M::unit(); });
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
                pre[i] = M::op(pre[i - 1], a[i]);
        _for_r (i, 1, n)
            if (i & mask)
                suf[i - 1] = M::op(a[i - 1], suf[i]);
        st.build(n >> LG, [&](int i) { return suf[i << LG]; });
    }
    X prod(int l, int r) const {
        if (l == r)
            return M::unit();
        r--;
        int x = l >> LG, y = r >> LG;
        if (x < y)
            return M::op(M::op(suf[l], st.prod(x + 1, y)), pre[r]);
        X t = a[l];
        _for (i, l + 1, r + 1)
            t = M::op(t, a[i]);
        return t;
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(M::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(M::unit()));
        if (r == 0)
            return 0;
        return bsearch([&](int l) { return check(prod(l, r)); }, r, -1);
    }
};