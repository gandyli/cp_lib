#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/tree_abelgroup.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(i64, a, n);
    Graph<int, true> g(n);
    _for (i, 1, n) {
        dR(int, p);
        g.add(p, i);
    }
    g.build();
    Tree tree(g);
    Tree_AbelGroup<decltype(tree), Monoid_Add<i64>, false, false, true> st(tree, a);
    _for (q) {
        dR(int, t, x);
        if (t == 0) {
            dR(int, y);
            st.multiply(x, y);
        }
        else {
            print(st.prod_subtree(x));
        }
    }
    return 0;
}