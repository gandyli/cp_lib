#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_A"

#include "all.hpp"
#include "graph/find_cycle.hpp"

int main() {
    dR(int, n, m);
    Graph<void, true> g(n);
    g.read_graph(m, 0);
    auto cycle = find_cycle(g);
    print(!cycle.vs.empty());
    return 0;
}