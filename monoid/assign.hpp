#pragma once
#include "template.hpp"

template <typename T, T none = inf<T>>
struct Monoid_Assign {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return b == none ? a : b; }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return none; }
    static constexpr bool commute() { return false; }
};