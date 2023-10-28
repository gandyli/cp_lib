#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C"

#include "all.hpp"
#include "graph/tree.hpp"

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
    Tree tree(g);
    dR(int, q);
    _for (q) {
        dR(int, x, y);
        print(tree.lca(x, y));
    }
    return 0;
}