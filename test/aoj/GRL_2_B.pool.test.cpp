#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_B"

#include "all.hpp"
#include "graph/minimum_cost_arborescence.hpp"

int main() {
    dR(int, n, m, r);
    auto g = read_graph<int, true>(n, m, 0);
    auto [cost, I] = MinimumCostArborescence(g, r);
    print(cost);
    return 0;
}