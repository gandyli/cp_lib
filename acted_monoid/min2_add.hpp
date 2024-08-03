#pragma once
#include "monoid/min2.hpp"
#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_Min2_Add {
    using Monoid_X = Monoid_Min2<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return {Min(inf<T>, x.first + a), Min(inf<T>, x.second + a)};
    }
};