#pragma once
#include "../template.hpp"

template <typename T>
std::tuple<T, T, T> exgcd(T a, T b) {
    if (!b)
        return {1, 0, a};
    auto [x, y, d] = exgcd(b, a % b);
    return {y, x - (a / b) * y, d};
}