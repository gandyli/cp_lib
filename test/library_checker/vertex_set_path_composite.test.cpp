#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

#include "all.hpp"
#include "graph/tree.hpp"
#include "graph/tree_monoid.hpp"
#include "modint/montgomery.hpp"
#include "monoid/affine.hpp"

int main() {
    using mint = MMInt998244353;
    using Mono = Monoid_Affine<mint>;
    dR(int, n, q);
    dRV(Mono::X, a, n);
    auto g = read_tree(n, 0);
    Tree tree(g);
    Tree_Monoid<decltype(tree), Mono> st(tree, a);
    _for (q) {
        dR(int, t, x, y, z);
        if (t == 0) {
            st.set(x, {y, z});
        }
        else {
            print(Mono::eval(st.prod_path(x, y), z));
        }
    }
    return 0;
}