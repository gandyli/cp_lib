#pragma once
#include "../io.hpp"

using UnweightedGraph = vvi;
template <typename T>
struct Edge {
    int src, to;
    T cost;

    Edge(int to, T cost): src(-1), to(to), cost(cost) {}
    Edge(int src, int to, T _cost): src(src), to(to), cost(cost) {}

    Edge& operator=(int x) {
        to = x;
        return *this;
    }

    operator int() const { return to; }
};
template <typename T>
using Edges = Vec<Edge<T>>;
template <typename T>
using WeightedGraph = Vec<Edges<T>>;

UnweightedGraph graph(int n, int m = -1, bool is_directed = false, bool is_1origin = true) {
    UnweightedGraph g(n);
    if (m == -1)
        m = n - 1;
    _for (m) {
        dR(int, x, y);
        if (is_1origin)
            x--, y--;
        g[x].eb(y);
        if (!is_directed)
            g[y].eb(x);
    }
    return g;
}

template <typename T>
WeightedGraph<T> wgraph(int n, int m = -1, bool is_directed = false, bool is_1origin = true) {
    WeightedGraph<T> g(n);
    if (m == -1)
        m = n - 1;
    _for (m) {
        dR(int, x, y);
        dR(T, c);
        if (is_1origin)
            x--, y--;
        g[x].eb(x, y, c);
        if (!is_directed)
            g[y].eb(y, x, c);
    }
    return g;
}

template <typename T>
Edges<T> esgraph(int n, int m, int is_weighted = true, bool is_1origin = true) {
    Edges<T> es;
    _for (m) {
        dR(int, x, y);
        T c;
        if (is_weighted)
            io.read(c);
        else
            c = 1;
        if (is_1origin)
            x--, y--;
        es.eb(x, y, c);
    }
    return es;
}

template <typename T>
Vec<Vec<T>> adjgraph(int n, int m, T INF, int is_weighted = true, bool is_directed = false, bool is_1origin = true) {
    VECI(d, n, n, INF);
    _for (m) {
        dR(int, x, y);
        T c;
        if (is_weighted)
            io.read(c);
        else
            c = 1;
        if (is_1origin)
            x--, y--;
        d[x][y] = c;
        if (!is_directed)
            d[y][x] = c;
    }
    return d;
}