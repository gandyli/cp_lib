#pragma once
#include "monoid/minmincnt2.hpp"
#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_MinMincnt2_Add {
    using Monoid_X = Monoid_MinMincnt2<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return {Min(inf<T>, x.min + a), Min(inf<T>, x.min2 + a), x.cnt, x.cnt2};
    }
};