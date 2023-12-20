#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include "all.hpp"
#include "ds/segtree.hpp"
#include "monoid/min.hpp"

int main() {
    dR(int, n, q);
    SegTree<Monoid_Min<i64>> st(n, [&](int) { return INT_MAX; });
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            st.set(x, y);
        }
        else {
            print(st.prod(x, y + 1));
        }
    }
    return 0;
}