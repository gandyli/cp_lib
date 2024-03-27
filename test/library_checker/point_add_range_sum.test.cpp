#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "all.hpp"
#include "ds/fenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    FenwickTree<Monoid_Add<i64>> bit(n, [&](int) { return io.read(); });
    _for (q) {
        dR(int, t, x, y);
        if (t == 0) {
            bit.multiply(x, y);
        }
        else {
            print(bit.prod(x, y));
        }
    }
    return 0;
}