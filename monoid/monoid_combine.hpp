#pragma once
#include "template.hpp"

template <typename... M>
struct Monoid_Combine {
    using value_type = std::tuple<typename M::value_type...>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        return [&]<auto... I>(std::index_sequence<I...>) -> X { return {M::op(std::get<I>(a), std::get<I>(b))...}; }(std::index_sequence_for<M...>{});
    }
    static constexpr X inverse(const X& x) {
        return [&]<auto... I>(std::index_sequence<I...>) -> X { return {M::inverse(std::get<I>(x))...}; }(std::index_sequence_for<M...>{});
    }
    static constexpr X power(const X& x, auto n) {
        return [&]<auto... I>(std::index_sequence<I...>) -> X { return {M::power(std::get<I>(x), n)...}; }(std::index_sequence_for<M...>{});
    }
    static constexpr X from_element(auto&& x) { return {M::from_element(x)...}; }
    static constexpr X unit() { return {M::unit()...}; }
    static constexpr bool commute() { return (M::commute() && ...); }
};