#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H"

#include "all.hpp"
#include "ds/lazy_segtree.hpp"
#include "acted_monoid/min_add.hpp"

int main() {
    dR(int, n, q);
    Lazy_SegTree<ActedMonoid_Min_Add<int>> st(n, [&](int) { return 0; });
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