#pragma once
#include "modint/barrett.hpp"
#include "monoid/mul.hpp"
#include "math/discrete_log.hpp"

template <Modint mint>
std::optional<mint> mod_log(mint a, mint b) {
    return discrete_log_monoid<Monoid_Mul<mint>>(a, b, [&](mint x) { return x.val(); }, 0, mint::mod());
}
template <Unsigned T>
std::optional<T> mod_log(T a, T b, T p) {
    SetBMod(T, p);
    auto r = mod_log<mint>(a, b);
    if (r)
        return r->val();
    return std::nullopt;
}