#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "ds/lazy_segtree.hpp"
#include "acted_monoid/sum_affine.hpp"

int main() {
    dR(u32, n, q);
    using mint = MMInt998244353;
    Lazy_SegTree<ActedMonoid_Sum_Affine<mint>> st(n, [&](int) { return io.read<u32>(); });
    _for (q) {
        dR(u32, t, l, r);
        if (t == 0) {
            dR(u32, x, y);
            st.apply(l, r, {x, y});
        }
        else {
            print(st.prod(l, r));
        }
    }
    return 0;
}