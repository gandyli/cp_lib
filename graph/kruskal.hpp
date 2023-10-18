#pragma once
#include "base.hpp"
#include "../ds/dsu.hpp"

template <typename T>
auto kruskal(Graph<T>& g) {
    int n = g.n, m = g.m;
    Vec<std::pair<T, int>> edges;
    _for (i, m) {
        auto& e = g.edges[i];
        edges.eb(e.cost, i);
    }
    sort(edges);
    Vec<bool> in(m);
    DSU dsu(n);
    T cost{};
    Graph<T> mst(n);
    for (auto&& [_, i]: edges)
        if (auto&& e = g.edges[i]; dsu.merge(e.from, e.to)) {
            in[i] = 1;
            mst.add(e.from, e.to, e.cost);
            cost += e.cost;
        }
    mst.build();
    return std::tuple{cost, std::move(in), std::move(mst)};
}