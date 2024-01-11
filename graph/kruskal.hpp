#pragma once
#include "graph/base.hpp"
#include "ds/unionfind.hpp"

template <typename T, typename G>
struct KruskalResult {
    T cost;
    Vec<bool> in;
    G mst;
};
template <typename T>
auto kruskal(const auto& g) {
    const int n = g.n, m = g.m;
    vi I(m);
    iota(all(I), 0);
    sort(I, [&](int i, int j) { return g.edges[i].cost < g.edges[j].cost; });
    UnionFind uf(n);
    T cost{};
    Vec<bool> in(m);
    std::decay_t<decltype(g)> mst(n, n - 1);
    foreach (i, I)
        if (auto&& e = g.edges[i]; uf.merge(e.from, e.to)) {
            in[i] = true;
            mst.add(e.from, e.to, e.cost);
            cost += e.cost;
        }
    mst.build();
    return KruskalResult{cost, std::move(in), std::move(mst)};
}