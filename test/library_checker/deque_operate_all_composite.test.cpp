#define PROBLEM "https://judge.yosupo.jp/problem/deque_operate_all_composite"

#include "all.hpp"
#include "ds/swag.hpp"
#include "modint/montgomery.hpp"
#include "monoid/affine.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, q);
    using Mono = Monoid_Affine<mint>;
    SWAG_deque<Mono> swag;
    _for (q) {
        dR(int, t);
        if (t == 0) {
            dR(int, a, b);
            swag.emplace_front(a, b);
        }
        if (t == 1) {
            dR(int, a, b);
            swag.emplace_back(a, b);
        }
        if (t == 2) {
            swag.pop_front();
        }
        if (t == 3) {
            swag.pop_back();
        }
        if (t == 4) {
            dR(int, x);
            print(Mono::eval(swag.prod(), x));
        }
    }
    return 0;
}