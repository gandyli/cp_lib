#define PROBLEM "https://atcoder.jp/contests/abc266/tasks/abc266_f"

#include "all.hpp"
#include "graph/unicyclic.hpp"

int main() {
    dR(int, n);
    auto g = read_graph(n, n);
    UnicyclicGraph ug(g);
    auto tree = ug.build().second;
    dR(int, q);
    _for (q) {
        dR(int, u, v), u--, v--;
        int u0 = tree.lca(u, ug.bottom), v0 = tree.lca(v, ug.bottom);
        Yes(u0 == v0);
    }
    return 0;
}