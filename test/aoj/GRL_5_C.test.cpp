#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C"

#include "all.hpp"
#include "tree/heavy_light_decomposition.hpp"

int main() {
    dR(int, n);
    Graph<int> g(n);
    _for (i, n) {
        dR(int, t);
        _for (t) {
            dR(int, v);
            g.add(i, v);
        }
    }
    g.build();
    HeavyLightDecomposition hld(g);
    dR(int, q);
    _for (q) {
        dR(int, x, y);
        writeln(hld.lca(x, y));
    }
    return 0;
}