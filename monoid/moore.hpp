#pragma once
#include "template.hpp"

// Moore's Voting Algorithm
template <typename T, T none = inf<T>>
struct Monoid_Moore {
    using value_type = std::pair<T, u32>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        auto [x0, c0] = a;
        auto [x1, c1] = b;
        if (x0 == x1)
            return {x0, c0 + c1};
        if (c0 > c1 || x1 == none)
            return {x0, c0 - c1};
        return {x1, c1 - c0};
    }
    static constexpr X from_element(auto&& x) { return {x, 1}; }
    static constexpr X unit() { return {none, 0}; }
    static constexpr bool commute() { return true; }
};