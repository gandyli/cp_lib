#define PROBLEM "https://atcoder.jp/contests/past202004-open/tasks/past202004_n"

#include "all.hpp"
#include "ds/dual_kdtree_monoid.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    using T = std::tuple<int, int, int, int>;
    dRV(T, a, n);
    vi X(q), Y(q);
    io.zipread(X, Y);
    Dual_KDTree_Monoid<Monoid_Add<i64>> tree(X, Y);
    foreach (x, y, d, c, a)
        tree.apply(x, x + d + 1, y, y + d + 1, c);
    _for (i, q)
        print(tree.get(i));
    return 0;
}