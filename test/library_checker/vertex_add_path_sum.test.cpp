#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/tree_monoid.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(i64, a, n);
    Graph<int> g(n);
    g.read_tree(false, 0);
    Tree tree(g);
    Tree_Monoid<decltype(tree), Monoid_Add<i64>> st(tree, a);
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            st.multiply(x, y);
        }
        else {
            print(st.prod_path(x, y));
        }
    }
    return 0;
}