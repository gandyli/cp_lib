#define PROBLEM "https://atcoder.jp/contests/past202010-open/tasks/past202010_m"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/dual_tree_monoid.hpp"
#include "monoid/assign.hpp"

int main() {
    dR(int, n, m);
    Graph g(n);
    g.read_tree();
    Tree tree(g);
    Dual_Tree_Monoid<decltype(tree), Monoid_Assign<int, 0>, true> seg(tree);
    _for (m) {
        dR(int, x, y, z), x--, y--;
        seg.apply_path(x, y, z);
    }
    io.displayArray(seg.get_all(), '\n');
    return 0;
}