#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "ds/dynamic_segtree.hpp"
#include "monoid/affine.hpp"

int main() {
    dR(int, n, q);
    using mint = MMInt998244353;
    using M = Monoid_Affine<mint>;
    using X = M::value_type;
    dRV(X, a, n);
    Dynamic_SegTree<M, false, int, 2 * ten(6)> st(0, n);
    using np = decltype(st)::np;
    np root = st.new_node(a);
    _for (q) {
        dR(int, t, x, y, z);
        if (t == 0) {
            st.set(root, x, {y, z});
        }
        else {
            print(M::eval(st.prod(root, x, y), z));
        }
    }
    return 0;
}