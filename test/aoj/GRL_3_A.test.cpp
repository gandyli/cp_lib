#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_A"

#include "all.hpp"
#include "graph/block_cut_tree.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph(n, m, 0);
    auto bct = block_cut_tree(g);
    _for (i, n)
        if (bct.deg(i) > 1)
            print(i);
    return 0;
}