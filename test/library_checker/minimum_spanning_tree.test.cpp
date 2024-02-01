#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include "all.hpp"
#include "graph/kruskal.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);
    g.read_graph(m, 0);
    auto [cost, in, _] = kruskal<i64>(g);
    print(cost);
    vi ans;
    _for (i, m)
        if (in[i])
            ans.eb(i);
    print(ans);
    return 0;
}