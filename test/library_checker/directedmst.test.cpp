#define PROBLEM "https://judge.yosupo.jp/problem/directedmst"

#include "all.hpp"
#include "graph/minimum_cost_arborescence.hpp"

int main() {
    dR(int, n, m, s);
    Graph<i64, true> g(n);
    g.read_graph(m, true, 0);

    auto [cost, I] = MinimumCostArborescence<4 * ten(5)>(g, s);
    vi par(n);
    par[s] = s;
    foreach (i, I) {
        auto&& e = g.edges[i];
        par[e.to] = e.from;
    }
    writeln(cost);
    writeln(par);
    return 0;
}