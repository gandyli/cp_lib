#pragma once
#include "template.hpp"

template <typename Monoid>
struct SqrtFenwickTree {
    using MX = Monoid;
    using X = MX::value_type;

    int n, m, bs;
    vc<X> dat1;
    vvc<X> dat2;
    SqrtFenwickTree() = default;
    SqrtFenwickTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    SqrtFenwickTree(const vc<T>& a) { build(a); }
    SqrtFenwickTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        bs = sqrt(n);
        m = n / bs + 1;
        dat1.assign(m + 1, MX::unit());
        dat2.resize(m);
        _for (i, m - 1)
            dat2[i].assign(bs + 1, MX::unit());
        dat2.back().assign(n - bs * (m - 1) + 1, MX::unit());
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        bs = sqrt(n);
        m = n / bs + 1;
        dat1.assign(m + 1, MX::unit());
        dat2.resize(m);
        _for (i, m) {
            int l = i * bs, r = min(n, (i + 1) * bs);
            dat2.assign(r - l + 1, MX::unit());
            _for (j, l, r) {
                dat2[i][j + 1 - l] = MX::op(dat2[i][j - l], f(j));
            }
            dat1[i + 1] = MX::op(dat1[i], dat2[i].back());
        }
    }
    void set(int i, const X& x) {
        multiply(i, MX::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        int p = i / bs;
        int l = p * bs, r = min(n, (p + 1) * bs);
        _for (j, i, r)
            dat2[p][j + 1 - l] = MX::op(dat2[p][j + 1 - l], x);
        _for (j, p, m)
            dat1[j + 1] = MX::op(dat1[j + 1], x);
    }
    X get(int i) const {
        int p = i / bs;
        return MX::op(dat2[p][i + 1 - p * bs], MX::inverse(dat2[p][i - p * bs]));
    }
    vc<X> get_all() const {
        vc<X> a(n);
        _for (i, n)
            a[i] = get(i);
        return a;
    }
    X prod(int i) const {
        int p = i / bs;
        return MX::op(dat1[p], dat2[p][i - p * bs]);
    }
    X prod(int l, int r) const { return MX::op(prod(r), MX::inverse(prod(l))); }
    X prod_all() const { return dat1.back(); }
};