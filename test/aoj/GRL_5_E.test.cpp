#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_E"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/lazy_tree_monoid.hpp"
#include "acted_monoid/sum_add.hpp"

int main() {
    dR(int, n);
    Graph g(n);
    _for (i, n) {
        dR(int, t);
        _for (t) {
            dR(int, v);
            g.add(i, v);
        }
    }
    g.build();
    Tree tree(g);
    Lazy_Tree_Monoid<decltype(tree), ActedMonoid_Sum_Add<i64>, true> st(tree);
    dR(int, q);
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, u, w);
            st.apply_path(0, u, w);
        }
        else {
            dR(int, u);
            print(st.prod_path(0, u));
        }
    }
    return 0;
}