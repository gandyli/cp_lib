#pragma once
#include "math/gcd.hpp"

template <typename T>
struct Monoid_Gcd {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return binary_gcd(a, b); }
    static constexpr X unit() { return {}; }
    static constexpr bool commute = true;
};