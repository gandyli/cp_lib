#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_D"

#include "all.hpp"
#include "ds/dual_segtree.hpp"
#include "monoid/assign.hpp"

int main() {
    dR(int, n, q);
    Dual_SegTree<Monoid_Assign<int, INT_MAX>> st(n);
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