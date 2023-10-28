#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#include "all.hpp"
#include "ds/dsu.hpp"

int main() {
    dR(int, n, q);
    DSU dsu(n);
    _for (q) {
        dR(int, t, x, y);
        if (t == 0)
            dsu.merge(x, y);
        else
            print(dsu.same(x, y));
    }
    return 0;
}