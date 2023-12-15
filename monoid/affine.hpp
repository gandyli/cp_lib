#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_Affine {
    using value_type = std::pair<T, T>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return {a.first * b.first, a.second * b.first + b.second}; }
    static constexpr X inverse(const X& x) {
        X t = 1 / x.first;
        return {t, -t * x.second};
    }
    static constexpr T eval(const X& x, T y) { return x.first * y + x.second; }
    static constexpr X unit() { return {1, 0}; }
    static constexpr bool commute = false;
};