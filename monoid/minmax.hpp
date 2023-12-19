#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_MinMax {
    using value_type = std::pair<T, T>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return {Min(a.first, b.first), Max(a.second, b.second)}; }
    static constexpr X from_element(const T& x) { return {x, x}; }
    static constexpr X unit() { return {inf<T>, -inf<T>}; }
    static constexpr bool commute = true;
};