#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "all.hpp"
#include "ds/wide_segtree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    Wide_SegTree<Monoid_Add<i64>> seg(n, [&](int) { return io.read(); });
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            seg.multiply(x, y);
        }
        else {
            print(seg.prod(x, y));
        }
    }
    return 0;
}