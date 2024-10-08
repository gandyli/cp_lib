#pragma once
#include "monoid/rollinghash.hpp"
#include "monoid/add.hpp"

template <typename mint = MInt61>
struct ActedMonoid_Rollinghash_Add {
    using Monoid_X = Monoid_Rollinghash<mint>;
    using Monoid_A = Monoid_Add<mint>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;

    static inline const mint invbase = (Monoid_X::base - 1).inv();
    static constexpr X act(const X& x, const A& a, int sz) {
        return {x.first, x.second + a * (x.first - 1) * invbase};
    }
};