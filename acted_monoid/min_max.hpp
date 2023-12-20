#pragma once
#include "monoid/min.hpp"
#include "monoid/max.hpp"

template <typename T>
struct ActedMonoid_Min_Max {
    using Monoid_X = Monoid_Min<T>;
    using Monoid_A = Monoid_Max<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return Max(x, a);
    }
};