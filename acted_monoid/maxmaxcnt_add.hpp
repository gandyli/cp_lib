#pragma once
#include "monoid/maxmaxcnt.hpp"
#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_MaxMaxcnt_Add {
    using Monoid_X = Monoid_MaxMaxcnt<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return {Max(-inf<T>, x.first + a), x.second};
    }
};