#pragma once
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

template <typename T>
auto val(const T& x) {
    if constexpr (Modint<T>)
        return x.val();
    else
        return x;
}