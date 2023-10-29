#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_Max {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return Max(a, b); }
    static constexpr T unit() { return -inf<T>; }
    static constexpr bool commute = true;
};