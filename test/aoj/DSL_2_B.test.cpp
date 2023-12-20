#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B"

#include "all.hpp"
#include "ds/segtree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    SegTree<Monoid_Add<int>> st(n);
    _for (q) {
        dR(int, t, x, y), x--;
        if (t == 0) {
            st.multiply(x, y);
        }
        else {
            print(st.prod(x, y));
        }
    }
    return 0;
}