#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E"

#include "all.hpp"
#include "ds/dual_segtree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    Dual_SegTree<Monoid_Add<int>> st(n);
    _for (q) {
        dR(int, t, x);
        if (t == 0) {
            dR(int, y, z);
            st.apply(x, y + 1, z);
        }
        else {
            print(st.get(x));
        }
    }
    return 0;
}