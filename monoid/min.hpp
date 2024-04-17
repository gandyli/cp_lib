#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_Min {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return Min(a, b); }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return inf<X>; }
    static constexpr bool commute() { return true; }
};