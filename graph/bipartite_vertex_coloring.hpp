#pragma once
#include "graph/base.hpp"
#include "ds/unionfind.hpp"

std::optional<vi> bipartite_vertex_coloring(const UndirectedGraph auto& g) {
    const int n = g.n;
    UnionFind uf(n * 2);
    foreach (e, g.edges) {
        int u = e.from, v = e.to;
        uf.merge(u + n, v);
        uf.merge(u, v + n);
    }
    vi col(n * 2, -1);
    _for (i, n)
        if (uf[i] == i && col[uf[i]] < 0) {
            col[uf[i]] = 0;
            col[uf[i + n]] = 1;
        }
    _for (i, n)
        col[i] = col[uf[i]];
    col.resize(n);
    _for (i, n)
        if (uf[i] == uf[i + n])
            return std::nullopt;
    return col;
}