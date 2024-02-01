#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include "all.hpp"
#include "graph/kruskal.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<int>(n, m, 0);
    auto [cost, in, _] = kruskal<i64>(g);
    print(cost);
    vi ans;
    _for (i, m)
        if (in[i])
            ans.eb(i);
    print(ans);
    return 0;
}