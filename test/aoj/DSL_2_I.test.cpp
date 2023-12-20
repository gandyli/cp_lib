#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_I"

#include "all.hpp"
#include "ds/lazy_segtree.hpp"
#include "acted_monoid/sum_assign.hpp"

int main() {
    dR(int, n, q);
    Lazy_SegTree<ActedMonoid_Sum_Assign<int>> st(n);
    _for (q) {
        dR(int, t, l, r), r++;
        if (t == 0) {
            dR(int, x);
            st.apply(l, r, x);
        }
        else {
            print(st.prod(l, r));
        }
    }
    return 0;
}