#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_Min {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return Min(a, b); }
    static constexpr T unit() { return inf<T>; }
    static constexpr bool commute = true;
};