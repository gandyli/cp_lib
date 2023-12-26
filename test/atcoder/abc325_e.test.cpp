#define PROBLEM "https://atcoder.jp/contests/abc325/tasks/abc325_e"

#include "all.hpp"
#include "graph/dijkstra.hpp"

int main() {
    dR(int, n, a, b, c);
    Graph<i64, true> g(n * 2);
    _for (i, n)
        _for (j, n) {
            dR(i64, x);
            g.add(i, j, x * a);
            g.add(i + n, j + n, x * b + c);
        }
    _for (i, n)
        g.add(i, i + n, 0);
    g.build();
    print(dijkstra_dense<i64>(g).dis.back());
    return 0;
}