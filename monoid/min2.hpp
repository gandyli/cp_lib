#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_Min2 {
    using value_type = std::pair<T, T>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        if (a.first == b.first)
            return {a.first, min(a.second, b.second)};
        if (a.first < b.first)
            return {a.first, min(a.second, b.first)};
        return {b.first, min(b.second, a.first)};
    }
    static constexpr X from_element(auto&& x) { return {x, inf<T>}; }
    static constexpr X unit() { return {inf<T>, inf<T>}; }
    static constexpr bool commute() { return true; }
};