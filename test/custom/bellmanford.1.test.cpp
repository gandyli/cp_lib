#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "graph/bellmanford.hpp"

void test() {
    const int n = 4000;
    Graph<int, true> g(n);
    _for (i, n - 2)
        g.add(i, i + 1, -100);
    g.add(n - 2, n - 1, 100);
    g.add(n - 1, 0, -100);
    g.build();
    auto dis = BellmanFord<int>(g).dis;
    assert(count(dis, -inf<int>) == n);
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}