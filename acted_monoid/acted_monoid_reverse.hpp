#pragma once
#include "monoid/monoid_reverse.hpp"

template <typename AM>
struct Acted_Monoid_Reverse {
    using Monoid_X = Monoid_Reverse<typename AM::Monoid_X>;
    using Monoid_A = AM::Monoid_A;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int sz) { return AM::act(x, a, sz); }
};