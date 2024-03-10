#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "all.hpp"
#include "ds/static_range_product.hpp"
#include "ds/sparse_table.hpp"
#include "monoid/min.hpp"

int main() {
    dR(int, n, q);
    vi fa(n);
    io.readArray(1 + all(fa));
    vi lid(n);
    _for_r (i, 1, n)
        lid[fa[i]] += lid[i] + 1;
    _for (i, 1, n)
        lid[i] = std::exchange(lid[fa[i]], lid[fa[i]] - lid[i] - 1);
    vi a(n);
    _for (i, 1, n) {
        a[lid[i] - 1] = fa[i];
    }
    Static_Range_Product<Sparse_Table, Monoid_Min<int>> st(a);
    _for (q) {
        dR(int, u, v);
        u = lid[u], v = lid[v];
        if (u > v)
            swap(u, v);
        print(st.prod(u, v));
    }
    return 0;
}