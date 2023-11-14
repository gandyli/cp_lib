#define PROBLEM "https://atcoder.jp/contests/arc157/tasks/arc157_a"

#include "all.hpp"
#include "graph/euler_walk.hpp"

int main() {
    dR(int, n, a, b, c, d);
    Graph<int, true> g(2);
    _for (a)
        g.add(0, 0);
    _for (b)
        g.add(0, 1);
    _for (c)
        g.add(1, 0);
    _for (d)
        g.add(1, 1);
    g.build();
    Yes(!euler_walk(g).vs.empty());
    return 0;
}