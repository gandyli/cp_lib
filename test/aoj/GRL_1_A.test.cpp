#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A"

#include "all.hpp"
#include "graph/dijkstra.hpp"

int main() {
    dR(int, n, m, s);
    Graph<int, true> g(n);
    g.read_graph(m, true, 0);

    auto [dis, par] = dijkstra<int>(g, s);
    _for (i, n) {
        if (dis[i] == inf<int>)
            print("INF");
        else
            print(dis[i]);
    }
    return 0;
}