#pragma once
#include "random/base.hpp"
#include "modint/mint61.hpp"

template <typename mint = MMInt61>
struct Monoid_Rollinghash {
    using value_type = std::pair<mint, mint>;
    using X = value_type;

    static u64 generateBase() { return rnd(1, mint::mod()); }
    static inline const mint base = generateBase();

    static constexpr X op(const X& a, const X& b) { return {a.first * b.first, a.second * b.first + b.second}; }
    static constexpr X from_element(auto&& x) { return {base, x}; }
    static constexpr X unit() { return {1, 0}; }
    static constexpr bool commute = false;
};