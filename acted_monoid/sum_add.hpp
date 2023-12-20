#include "monoid/add.hpp"

template <typename T>
struct ActedMonoid_Sum_Add {
    using Monoid_X = Monoid_Add<T>;
    using Monoid_A = Monoid_Add<T>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int sz) {
        return x + a * sz;
    }
};