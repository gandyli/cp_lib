#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_B"

#include "all.hpp"
#include "graph/two_edge_component.hpp"

int main() {
    dR(int, n, m);
    Graph<bool> g(n);
    g.read_graph(m, false, 0);
    auto [bcc, comp] = two_edge_component(g);
    Vec<pi> ans;
    foreach (e, g.edges)
        if (comp[e.from] != comp[e.to]) {
            if (e.from < e.to)
                ans.eb(e.from, e.to);
            else
                ans.eb(e.to, e.from);
        }
    sort(ans);
    foreach (x, ans)
        writeln(x);
    return 0;
}