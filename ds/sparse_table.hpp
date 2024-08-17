#pragma once
#include "math/binary_search.hpp"

template <typename Monoid>
struct Sparse_Table {
    using MX = Monoid;
    using X = MX::value_type;

    int n, lg;
    vvc<X> st;

    Sparse_Table() = default;
    Sparse_Table(int n) { build(n); }
    template <std::convertible_to<X> T>
    Sparse_Table(const vc<T>& a) { build(a); }
    Sparse_Table(int n, std::invocable<int> auto&& f) { build(n, f); }

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
        st.resize(lg);
        st[0].resize(n);
        _for (i, n)
            st[0][i] = f(i);
        _for (i, lg - 1) {
            st[i + 1].resize(len(st[i]) - (1 << i));
            _for (j, len(st[i]) - (1 << i))
                st[i + 1][j] = MX::op(st[i][j], st[i][j + (1 << i)]);
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return MX::unit();
        if (l + 1 == r)
            return st[0][l];
        int k = 31 - __builtin_clz(r - l - 1);
        return MX::op(st[k][l], st[k][r - (1 << k)]);
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