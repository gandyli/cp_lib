#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"

#include "all.hpp"
#include "graph/twosat.hpp"

int main() {
    dR(str, p, cnf);
    dR(int, n, m);
    TwoSat ts(n);
    _for (i, m) {
        dR(int, a, b, O);
        if (a > 0)
            a--;
        if (b > 0)
            b--;
        ts.add(a, b);
    }
    auto values = ts.solve();
    if (!values) {
        print("s UNSATISFIABLE");
    }
    else {
        print("s SATISFIABLE");
        vi ans(n);
        _for (i, n) {
            if ((*values)[i])
                ans[i] = i + 1;
            else
                ans[i] = -(i + 1);
        }
        print("v", ans, 0);
    }
    return 0;
}