#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_A"

#include "all.hpp"
#include "graph/find_cycle.hpp"

int main() {
    dR(int, n, m);
    Graph<int, true> g(n);
    g.read_graph(m, false, 0);
    auto cycle = find_cycle(g);
    if (cycle.vs.empty())
        writeln(0);
    else
        writeln(1);
    return 0;
}