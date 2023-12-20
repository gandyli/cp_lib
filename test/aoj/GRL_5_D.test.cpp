#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/tree_monoid.hpp"
#include "monoid/add.hpp"

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
    Tree_Monoid<decltype(tree), Monoid_Add<int>, true> st(tree);
    dR(int, q);
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, u, w);
            st.multiply(tree.v_to_e(u), w);
        }
        else {
            dR(int, u);
            print(st.prod_path(0, u));
        }
    }
    return 0;
}