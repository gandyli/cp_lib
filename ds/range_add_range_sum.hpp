#pragma once
#include "ds/fenwicktree.hpp"
#include "monoid/monoid_combine.hpp"

template <typename Monoid>
struct Range_Add_Range_Sum {
    using M = Monoid;
    using X = M::value_type;
    FenwickTree<Monoid_Combine<M, M>> bit;
    Range_Add_Range_Sum() = default;
    Range_Add_Range_Sum(int n) { build(n); }
    template <std::convertible_to<X> T>
    Range_Add_Range_Sum(const vc<T>& a) { build(a); }
    Range_Add_Range_Sum(int n, std::invocable<int> auto&& f) { build(n, f); }
    void build(int n) { bit.build(n); }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        bit.build(n, [&](int i) { return std::tuple{f(i), M::unit()}; });
    }
    void apply(int l, int r, const X& x) {
        X y = M::inverse(x);
        bit.multiply(l, {M::power(y, l), x});
        bit.multiply(r, {M::power(x, r), y});
    }
    X prod(int l, int r) {
        auto [a, b] = bit.prod(l);
        auto [c, d] = bit.prod(r);
        X x = M::op(M::power(b, l), a);
        X y = M::op(M::power(d, r), c);
        return M::op(M::inverse(x), y);
    }
};