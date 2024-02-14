#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "ds/dynamic_lazy_segtree.hpp"
#include "acted_monoid/sum_affine.hpp"

int main() {
    dR(u32, n, q);
    using mint = MMInt998244353;
    dRV(mint, a, n);
    Dynamic_Lazy_SegTree<ActedMonoid_Sum_Affine<mint>, false> st(0, n);
    using np = decltype(st)::np;
    np root = st.new_node(a);
    _for (q) {
        dR(u32, t, l, r);
        if (t == 0) {
            dR(u32, x, y);
            root = st.apply(root, l, r, {x, y});
        }
        else {
            print(st.prod(root, l, r));
        }
    }
    return 0;
}