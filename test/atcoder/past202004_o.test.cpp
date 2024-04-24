#define PROBLEM "https://atcoder.jp/contests/past202004-open/tasks/past202004_o"

#include "all.hpp"
#include "graph/static_tree_monoid.hpp"
#include "graph/tree.hpp"
#include "graph/kruskal.hpp"
#include "monoid/max.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<int>(n, m);
    auto [cost, in, mst] = kruskal<i64>(g);
    Tree tree(mst);
    Static_Tree_Monoid<decltype(tree), Monoid_Max<int>, true> st(tree, [&](int i) { return mst.edges[i].cost; });
    _for (i, m) {
        if (in[i]) {
            print(cost);
        }
        else {
            auto&& e = g.edges[i];
            print(cost + e.cost - st.prod_path(e.from, e.to));
        }
    }
    return 0;
}