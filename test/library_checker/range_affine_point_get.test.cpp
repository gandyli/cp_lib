#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "ds/dual_segtree.hpp"
#include "monoid/affine.hpp"

int main() {
    dR(u32, n, q);
    using mint = MMInt998244353;
    dRV(mint, a, n);
    using Mono = Monoid_Affine<mint>;
    Dual_SegTree<Mono> st(n);
    _for (q) {
        dR(u32, t);
        if (t == 0) {
            dR(u32, l, r, x, y);
            st.apply(l, r, {x, y});
        }
        else {
            dR(u32, p);
            auto [x, y] = st.get(p);
            print(x * a[p] + y);
        }
    }
    return 0;
}