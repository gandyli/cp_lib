#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "ds/segtree.hpp"
#include "monoid/affine.hpp"

int main() {
    dR(int, n, q);
    using mint = MMInt998244353;
    using Mono = Monoid_Affine<mint>;
    SegTree<Mono> st(n, [&](int) -> Mono::X {
        dR(int, x, y);
        return {x, y};
    });
    _for (q) {
        dR(int, t, x, y, z);
        if (t == 0) {
            st.set(x, {y, z});
        }
        else {
            print(Mono::eval(st.prod(x, y), z));
        }
    }
    return 0;
}