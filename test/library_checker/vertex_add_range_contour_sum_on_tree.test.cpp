#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_range_contour_sum_on_tree"

#include "all.hpp"
#include "graph/contour_query_range.hpp"
#include "ds/fenwicktree.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    dRV(i64, a, n);
    auto g = read_tree(n, 0);
    Contour_Query_Range<decltype(g)> CQ(g);
    vc<i64> dat(len(CQ));
    _for (i, n)
        foreach (p, CQ.get_indices(i))
            dat[p] += a[i];
    FenwickTree<Monoid_Add<i64>> ft(dat);
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, u, x);
            a[u] += x;
            foreach (p, CQ.get_indices(u))
                ft.multiply(p, x);
        }
        else {
            dR(int, u, l, r);
            i64 ans = 0;
            if (l <= 0 && 0 < r)
                ans += a[u];
            foreach (a, b, CQ.get_contour_range(u, l, r))
                ans += ft.prod(a, b);
            print(ans);
        }
    }
    return 0;
}