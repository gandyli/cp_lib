#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A"

#include "all.hpp"
#include "graph/kruskal.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);
    g.read_graph(m, 0);
    print(kruskal<int>(g).cost);
    return 0;
}