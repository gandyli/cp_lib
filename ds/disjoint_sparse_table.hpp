#pragma once
#include "math/binary_search.hpp"

template <typename Monoid>
struct Disjoint_Sparse_Table {
    using MX = Monoid;
    using X = MX::value_type;

    int n, lg;
    vvc<X> st;
    Disjoint_Sparse_Table() = default;
    Disjoint_Sparse_Table(int n) { build(n); }
    template <std::convertible_to<X> T>
    Disjoint_Sparse_Table(const vc<T>& a) { build(a); }
    Disjoint_Sparse_Table(int n, std::invocable<int> auto&& f) { build(n, f); }

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
        _for (i, 1, lg) {
            auto&& v = st[i];
            v = st[0];
            int b = 1 << i;
            _for (m, b, n + 1, b << 1) {
                int l = m - b, r = Min(n, m + b);
                _for_r (j, l, m - 1)
                    v[j] = MX::op(v[j], v[j + 1]);
                _for (j, m, r - 1)
                    v[j + 1] = MX::op(v[j], v[j + 1]);
            }
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return MX::unit();
        r--;
        if (l == r)
            return st[0][l];
        int k = 31 - __builtin_clz(l ^ r);
        return MX::op(st[k][l], st[k][r]);
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