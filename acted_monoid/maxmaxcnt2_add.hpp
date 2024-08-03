#pragma once
#include "monoid/maxmaxcnt2.hpp"
#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_MinMincnt2_Add {
    using Monoid_X = Monoid_MaxMaxcnt2<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return {Max(-inf<T>, x.max + a), Max(-inf<T>, x.max2 + a), x.cnt, x.cnt2};
    }
};