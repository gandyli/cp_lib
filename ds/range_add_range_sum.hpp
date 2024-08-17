#pragma once
#include "ds/fenwicktree.hpp"
#include "monoid/monoid_combine.hpp"

template <typename Monoid>
struct Range_Add_Range_Sum {
    using MX = Monoid;
    using X = MX::value_type;
    FenwickTree<Monoid_Combine<MX, MX>> bit;
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
        bit.build(n, [&](int i) { return std::tuple{f(i), MX::unit()}; });
    }
    void apply(int l, int r, const X& x) {
        X y = MX::inverse(x);
        bit.multiply(l, {MX::power(y, l), x});
        bit.multiply(r, {MX::power(x, r), y});
    }
    X prod(int l, int r) const {
        auto [a, b] = bit.prod(l);
        auto [c, d] = bit.prod(r);
        X x = MX::op(MX::power(b, l), a);
        X y = MX::op(MX::power(d, r), c);
        return MX::op(MX::inverse(x), y);
    }
};