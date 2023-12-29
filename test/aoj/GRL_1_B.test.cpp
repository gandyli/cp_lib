#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B"

#include "all.hpp"
#include "graph/bellmanford.hpp"

int main() {
    dR(int, n, m, s);
    Graph<int, true> g(n);
    g.read_graph(m, true, 0);

    auto [dis, par] = BellmanFord<int>(g, s);
    if (min(dis) == -inf<int>) {
        print("NEGATIVE CYCLE");
        return 0;
    }
    _for (i, n) {
        if (dis[i] == inf<int>)
            print("INF");
        else
            print(dis[i]);
    }
    return 0;
}