#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C"

#include "all.hpp"
#include "graph/strongly_connected_component.hpp"

int main() {
    dR(int, n, m);
    Graph<bool, true> g(n);
    g.read_graph(m, false, 0);
    auto [scc, comp] = strongly_connected_component(g);
    dR(int, q);
    _for (q) {
        dR(int, x, y);
        writeln(comp[x] == comp[y]);
    }
    return 0;
}