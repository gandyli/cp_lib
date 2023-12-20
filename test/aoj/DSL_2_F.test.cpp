#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F"

#include "all.hpp"
#include "ds/lazy_segtree.hpp"
#include "acted_monoid/min_assign.hpp"

int main() {
    dR(int, n, q);
    Lazy_SegTree<ActedMonoid_Min_Assign<i64>> st(n, [&](int) { return INT_MAX; });
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