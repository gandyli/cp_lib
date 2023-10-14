#pragma once
#include "../graph/bellmanford.hpp"

template <typename T>
bool negative_cycle(const WeightedGraph<T>& g) {
    auto [dis, par] = BellmanFord(g);
    return min(dis) == -inf<T>;
}