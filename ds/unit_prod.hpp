#pragma once
#include "template.hpp"

template <typename Monoid>
struct Unit_Prod {
    constexpr auto operator()(i64, i64) const { return Monoid::unit(); }
};