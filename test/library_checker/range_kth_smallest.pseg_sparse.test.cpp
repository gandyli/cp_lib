#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include "all.hpp"
#include "ds/dynamic_segtree_sparse.hpp"
#include "monoid/add.hpp"
#include "math/binary_search.hpp"

int main() {
    dR(int, n, m);
    dRV(int, a, n);
    Dynamic_SegTree_Sparse<Monoid_Add<int>, true, 4 * ten(6)> seg(0, n);
    using Node = decltype(seg)::Node;
    auto I = argsort(a);
    vc<Node*> roots{seg.new_node()};
    _for (i, n)
        roots.eb(seg.set(roots.back(), I[i], 1));
    _for (m) {
        dR(int, l, r, k);
        auto check = [&](auto mi) { return seg.prod(roots[mi], l, r) <= k; };
        print(a[I[bsearch(check, 0, n)]]);
    }
    return 0;
}