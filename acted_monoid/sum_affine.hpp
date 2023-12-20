#pragma once
#include "monoid/add.hpp"
#include "monoid/affine.hpp"

template <typename T>
struct ActedMonoid_Sum_Affine {
    using Monoid_X = Monoid_Add<T>;
    using Monoid_A = Monoid_Affine<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int sz) {
        return x * a.first + sz * a.second;
    }
};