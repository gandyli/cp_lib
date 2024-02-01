#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "all.hpp"
#include "graph/fast_lca.hpp"

int main() {
    dR(int, n, q);
    Graph<void, true> g(n);
    _for (i, 1, n) {
        dR(int, p);
        g.add(p, i);
    }
    g.build();
    Fast_Lca_Tree tree(g);
    _for (q) {
        dR(int, u, v);
        print(tree.lca(u, v));
    }
    return 0;
}