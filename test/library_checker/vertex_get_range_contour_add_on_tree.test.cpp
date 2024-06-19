#define PROBLEM "https://judge.yosupo.jp/problem/vertex_get_range_contour_add_on_tree"

#include "all.hpp"
#include "graph/contour_query_range.hpp"
#include "ds/fenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(i64, a, n);
    auto g = read_tree(n, 0);
    Contour_Query_Range<decltype(g)> CQ(g);
    FenwickTree<Monoid_Add<i64>> ft(len(CQ) + 1);
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, u, l, r, x);
            foreach (a, b, CQ.get_contour_range(u, l, r))
                ft.multiply(a, x), ft.multiply(b, -x);
            if (l <= 0 && 0 < r)
                a[u] += x;
        }
        else {
            dR(int, u);
            i64 ans = a[u];
            foreach(p, CQ.get_indices(u))
                ans += ft.prod(p + 1);
            print(ans);
        }
    }
    return 0;
}