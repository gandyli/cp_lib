#pragma once
#include "monoid/minmax.hpp"
#include "monoid/mul.hpp"

template <typename T>
struct ActedMonoid_MinMax_Mul {
    using Monoid_X = Monoid_MinMax<T>;
    using Monoid_A = Monoid_Mul<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        if (a == 0)
            return {};
        auto&& [mn, mx] = x;
        if (a > 0)
            return {mn * a, mx * a};
        return {mx * a, mn * a};
    }
};