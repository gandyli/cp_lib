#pragma once
#include "monoid/minmincnt.hpp"
#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_MinMincnt_Add {
    using Monoid_X = Monoid_MinMincnt<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return {Min(inf<X>, x.first + a), x.second};
    }
};