#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_A"

#include "all.hpp"
#include "graph/block_cut_tree.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);
    g.read_graph(m, false, 0);

    auto bct = block_cut_tree(g);
    _for (i, n)
        if (bct.deg(i) > 1)
            writeln(i);
    return 0;
}