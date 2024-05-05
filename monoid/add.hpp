#pragma once

template <typename T>
struct Monoid_Add {
    using value_type = T;
    using X = value_type;
    static constexpr auto op(auto&& a, auto&& b) { return a + b; }
    static constexpr X inverse(const X& x) { return -x; }
    static constexpr X power(const X& x, auto n) { return n * x; }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return {}; }
    static constexpr bool commute() { return true; }
};