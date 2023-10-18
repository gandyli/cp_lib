#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "all.hpp"
#include "tree/heavy_light_decomposition.hpp"

int main() {
    dR(int, n, q);
    Graph<bool, false> g(n);
    _for (i, 1, n) {
        dR(int, p);
        g.add(p, i);
    }
    g.build();
    HeavyLightDecomposition hld(g);
    _for (q) {
        dR(int, u, v);
        writeln(hld.lca(u, v));
    }
    return 0;
}