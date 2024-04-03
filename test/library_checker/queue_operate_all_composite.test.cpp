#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

#include "all.hpp"
#include "ds/swag.hpp"
#include "modint/montgomery.hpp"
#include "monoid/affine.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, q);
    using Mono = Monoid_Affine<mint>;
    SWAG<Mono> swag;
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, a, b);
            swag.emplace(a, b);
        }
        if (t == 1) {
            swag.pop();
        }
        if (t == 2) {
            dR(int, x);
            print(Mono::eval(swag.prod(), x));
        }
    }
    return 0;
}