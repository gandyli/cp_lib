#pragma once
#include "template.hpp"
#include "math/binary_search.hpp"

template <typename Monoid>
struct Sparse_Table {
    using M = Monoid;
    using X = M::value_type;

    int n, lg;
    Vec<Vec<X>> st;

    Sparse_Table() = default;
    Sparse_Table(int n) { build(n); }
    Sparse_Table(const Vec<X>& a) { build(a); }
    Sparse_Table(int n, auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return M::unit(); });
    }
    void build(const Vec<X>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, auto&& f) {
        this->n = n;
        lg = 1;
        while ((1 << lg) < n)
            lg++;
        st.resize(lg);
        st[0].resize(n);
        _for (i, n)
            st[0][i] = f(i);
        _for (i, lg - 1) {
            st[i + 1].resize(len(st[i]) - (1 << i));
            _for (j, len(st[i]) - (1 << i))
                st[i + 1][j] = M::op(st[i][j], st[i][j + (1 << i)]);
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return M::unit();
        if (l + 1 == r)
            return st[0][l];
        int k = 31 - __builtin_clz(r - l - 1);
        return M::op(st[k][l], st[k][r - (1 << k)]);
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