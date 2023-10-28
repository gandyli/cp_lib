#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A"

#include "all.hpp"
#include "graph/kruskal.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);

    g.read_graph(m, true, 0);

    auto [cost, _0, _1] = kruskal(g);
    print(cost);
    return 0;
}