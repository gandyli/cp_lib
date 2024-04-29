#pragma once

template <typename Monoid>
struct ActedSet_From_Monoid {
    using Monoid_A = Monoid;
    using A = Monoid::value_type;
    using S = A;
    static constexpr S act(const S& x, const A& a) { return Monoid::op(x, a); }
};