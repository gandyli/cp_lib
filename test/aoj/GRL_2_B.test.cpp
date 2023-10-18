#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_B"

#include "all.hpp"
#include "graph/minimum_cost_arborescence.hpp"

int main() {
    dR(int, n, m, r);
    Graph<int, true> g(n);
    g.read_graph(m, true, 0);

    auto [cost, I] = MinimumCostArborescence<1500>(g, r);
    writeln(cost);
    return 0;
}