#define PROBLEM "https://judge.yosupo.jp/problem/manhattanmst"

#include "all.hpp"
#include "graph/manhattan_mst.hpp"

int main() {
    dR(int, n);
    dRV(pi, a, n);
    auto mst = manhattan_mst(a);
    i64 ans = 0;
    foreach (e, mst.edges)
        ans += e.cost;
    print(ans);
    foreach (e, mst.edges)
        print(e.from, e.to);
    return 0;
}