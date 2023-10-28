#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#include "all.hpp"
#include "graph/two_edge_component.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);
    g.read_graph(m, false, 0);
    auto [bcc, comp] = two_edge_component(g);
    vvi ans(bcc);
    _for (i, n)
        ans[comp[i]].eb(i);
    print(bcc);
    foreach (t, ans)
        print(len(t), t);
    return 0;
}