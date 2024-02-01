#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A"

#include "all.hpp"
#include "graph/kruskal.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<int>(n, m, 0);
    print(kruskal<int>(g).cost);
    return 0;
}