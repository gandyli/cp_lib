#pragma once
#include "monoid/min.hpp"

template <typename T>
struct ActedMonoid_Min_Min {
    using Monoid_X = Monoid_Min<T>;
    using Monoid_A = Monoid_Min<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return Min(x, a);
    }
};