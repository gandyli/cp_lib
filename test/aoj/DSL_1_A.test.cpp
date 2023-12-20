#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A"

#include "all.hpp"
#include "ds/unionfind.hpp"

int main() {
    dR(int, n, q);
    UnionFind uf(n);
    _for (q) {
        dR(int, t, x, y);
        if (t == 0)
            uf.merge(x, y);
        else
            print(uf.same(x, y));
    }
    return 0;
}