#pragma once
#include "monoid/monoid_combine.hpp"

template <typename... AM>
struct ActedMonoid_Combine {
    using Monoid_X = Monoid_Combine<typename AM::Monoid_X...>;
    using Monoid_A = std::tuple_element_t<0, std::tuple<typename AM::Monoid_A...>>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;

    static_assert((std::is_same_v<typename AM::Monoid_A, Monoid_A> && ...));
    static constexpr X act(const X& x, const A& a, int sz) {
        return [&]<auto... I>(std::index_sequence<I...>) -> X { return {AM::act(std::get<I>(x), a, sz)...}; }(std::index_sequence_for<AM...>{});
    }
};