#pragma once
#include "template.hpp"

template <typename T, int N>
struct Monoid_AddArray {
    using value_type = std::array<T, N>;
    using X = value_type;
    static constexpr X op(X a, const X& b) {
        _for (i, N)
            a[i] += b[i];
        return a;
    }
    static constexpr X inverse(X x) {
        foreach (x, x)
            x = -x;
        return x;
    }
    static constexpr X power(X x, auto n) {
        foreach (x, x)
            x *= n;
        return x;
    }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return {}; }
    static constexpr bool commute() { return true; }
};