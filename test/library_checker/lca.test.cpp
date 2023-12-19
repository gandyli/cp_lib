#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "all.hpp"
#include "graph/tree.hpp"

int main() {
    dR(int, n, q);
    Graph g(n);
    _for (i, 1, n) {
        dR(int, p);
        g.add(p, i);
    }
    g.build();
    Tree tree(g);
    _for (q) {
        dR(int, u, v);
        print(tree.lca(u, v));
    }
    return 0;
}