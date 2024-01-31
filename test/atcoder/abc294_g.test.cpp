#define PROBLEM "https://atcoder.jp/contests/abc294/tasks/abc294_g"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/tree_abelgroup.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n);
    Graph g(n);
    g.read_tree(true);
    Tree tree(g);
    Tree_AbelGroup<decltype(tree), Monoid_Add<i64>, true, true, false> seg(tree, [&](int i) { return g.edges[i].cost; });
    dR(int, q);
    _for (q) {
        dR(int, t, x, y);
        if (t == 1) {
            x--;
            seg.multiply(x, y - g.edges[x].cost);
            g.edges[x].cost = y;
        }
        else {
            x--, y--;
            print(seg.prod_path(x, y));
        }
    }
    return 0;
}