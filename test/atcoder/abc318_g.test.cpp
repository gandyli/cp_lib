#define PROBLEM "https://atcoder.jp/contests/abc318/tasks/abc318_g"

#include "all.hpp"
#include "flow/maxflow.hpp"

int main() {
    dR(int, n, m);
    dR(int, a, b, c), a--, b--, c--;

    int s = n * 2, t = n * 2 + 1;
    MaxFlow<int> mf(n * 2 + 2, s, t);

    _for (i, n)
        mf.add(i, i + n, 1);
    mf.add(b, b + n, 1);

    mf.add(s, b, 2);
    mf.add(a + n, t, 1);
    mf.add(c + n, t, 1);

    _for (m) {
        dR(int, u, v), u--, v--;
        mf.add(u + n, v, 1);
        mf.add(v + n, u, 1);
    }
    mf.build();
    Yes(mf.flow() == 2);
    return 0;
}