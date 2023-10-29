#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A"

#include "all.hpp"
#include "flow/maxflow.hpp"

int main() {
    dR(int, n, m);
    MaxFlow<int> mf(n, 0, n - 1);
    _for (m) {
        dR(int, u, v, c);
        mf.add(u, v, c);
    }
    mf.build();
    writeln(mf.flow());
    return 0;
}