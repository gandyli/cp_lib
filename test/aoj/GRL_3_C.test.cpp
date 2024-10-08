#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C"

#include "all.hpp"
#include "graph/strongly_connected_component.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<void, true>(n, m, 0);
    auto [scc, comp] = strongly_connected_component(g);
    dR(int, q);
    _for (q) {
        dR(int, x, y);
        print(comp[x] == comp[y]);
    }
    return 0;
}