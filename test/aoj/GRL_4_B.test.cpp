#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_B"

#include "all.hpp"
#include "graph/topological_sort.hpp"

int main() {
    dR(int, n, m);
    Graph<void, true> g(n);
    g.read_graph(m, 0);
    auto V = toposort(g);
    io.displayArray(V, '\n');
    return 0;
}