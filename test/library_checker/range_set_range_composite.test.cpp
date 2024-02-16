#define PROBLEM "https://judge.yosupo.jp/problem/range_set_range_composite"

#include "all.hpp"
#include "ds/range_assignment_segtree.hpp"
#include "monoid/affine.hpp"
#include "modint/montgomery.hpp"

int main() {
    dR(int, n, q);
    using mint = MMInt998244353;
    using Mono = Monoid_Affine<mint>;
    Range_Assignment_SegTree<Mono> seg(n, [&](int) {
        dR(mint, a, b);
        return std::pair{a, b};
    });
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, l, r, c, d);
            seg.apply(l, r, {c, d});
        } else {
            dR(int, l, r, x);
            print(Mono::eval(seg.prod(l, r), x));
        }
    }
    return 0;
}