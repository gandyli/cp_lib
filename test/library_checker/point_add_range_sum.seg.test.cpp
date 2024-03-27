#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "all.hpp"
#include "ds/segtree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    SegTree<Monoid_Add<i64>> st(n, [&](int) { return io.read(); });
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            st.multiply(x, y);
        }
        else {
            print(st.prod(x, y));
        }
    }
    return 0;
}