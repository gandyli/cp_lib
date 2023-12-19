#pragma once

template <typename T>
struct Monoid_And {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return a & b; }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return -1; }
    static constexpr bool commute = true;
};