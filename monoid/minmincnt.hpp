#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_MinMincnt {
    using value_type = std::pair<T, T>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        if (a.first < b.first)
            return a;
        if (a.first > b.first)
            return b;
        return {a.first, a.second + b.second};
    }
    static constexpr X from_element(auto&& x) { return {x, 1}; }
    static constexpr X unit() { return {inf<T>, 0}; }
    static constexpr bool commute = true;
};