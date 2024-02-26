#pragma once
#include "graph/strongly_connected_component.hpp"

struct TwoSat {
    Graph<void, true> g;
    vi values;

    TwoSat(int n): g(n << 1), values(n, -1) {}
    void add(int a, int b) {
        a = a >= 0 ? a << 1 | 1 : ~a << 1;
        b = b >= 0 ? b << 1 | 1 : ~b << 1;
        g.add(a ^ 1, b);
        g.add(b ^ 1, a);
    }
    void set(int a) {
        if (a >= 0)
            values[a] = 1;
        else
            values[~a] = 0;
        a = a >= 0 ? a << 1 | 1 : ~a << 1;
        g.add(a ^ 1, a);
    }
    std::optional<vi> solve() {
        g.build();
        int n = len(values);
        auto comp = strongly_connected_component(g).comp;
        _for (i, n) {
            if (comp[i << 1] == comp[i << 1 | 1])
                return std::nullopt;
            values[i] = comp[i << 1] < comp[i << 1 | 1];
        }
        return values;
    }
};