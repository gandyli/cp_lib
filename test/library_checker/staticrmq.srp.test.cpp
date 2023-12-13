#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "all.hpp"
#include "ds/sparse_table.hpp"
#include "ds/static_range_product.hpp"
#include "monoid/min.hpp"

int main() {
    dR(int, n, m);
    Static_Range_Product<Sparse_Table, Monoid_Min<int>> st(n, [&](int) { return io.read<int>(); });
    _for (m) {
        dR(int, l, r);
        print(st.prod(l, r));
    }
    return 0;
}