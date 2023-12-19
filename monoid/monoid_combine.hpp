#pragma once
#include "template.hpp"

template <typename... M>
struct Monoid_Combine {
    using value_type = std::tuple<typename M::value_type...>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        return [&]<auto... I>(std::index_sequence<I...>) { return std::tuple(M::op(std::get<I>(a), std::get<I>(b))...); }(std::index_sequence_for<M...>{});
    }
    static constexpr X from_element(auto&& x) { return {M::from_element(x)...}; }
    static constexpr X unit() { return {M::unit()...}; }
    static constexpr bool commute = (M::commute && ...);
};