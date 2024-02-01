#define PROBLEM "https://judge.yosupo.jp/problem/k_shortest_walk"

#include "all.hpp"
#include "graph/K_shortest_walk.hpp"

int main() {
    dR(int, n, m, s, t, k);
    Graph<int, true> g(n);
    g.read_graph(m, 0);
    auto ret = K_shortest_walk<i64, 5000000>(g, s, t, k);
    foreach (r, ret)
        if (r == inf<i64>)
            r = -1;
    io.displayArray(ret, '\n');
    return 0;
}