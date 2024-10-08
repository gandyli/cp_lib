#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include "all.hpp"
#include "graph/strongly_connected_component.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<void, true>(n, m, 0);
    auto [scc, comp] = strongly_connected_component(g);
    vvi ans(scc);
    _for (i, n)
        ans[comp[i]].eb(i);
    print(len(ans));
    foreach (x, ans)
        print(len(x), x);
    return 0;
}