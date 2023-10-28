#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_B"

#include "all.hpp"
#include "graph/utility.hpp"

int main() {
    dR(int, n);
    Graph<int> g(n);
    g.read_tree(true, 0);
    auto [d1, _0] = bfs01<int>(g, 0);
    int u = max_element(d1) - d1.begin();
    auto [d2, _1] = bfs01<int>(g, u);
    int v = max_element(d2) - d2.begin();
    auto [d3, _2] = bfs01<int>(g, v);
    _for (i, n)
        print(max(d2[i], d3[i]));
    return 0;
}