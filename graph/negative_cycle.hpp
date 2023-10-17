#pragma once
#include "bellmanford.hpp"

template <typename T>
bool negative_cycle(const auto& g) {
    auto [dis, par] = BellmanFord<T>(g);
    return min(dis) == -inf<T>;
}