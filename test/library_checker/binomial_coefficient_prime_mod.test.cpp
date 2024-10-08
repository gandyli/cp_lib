#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include "all.hpp"
#include "modint/binomial.hpp"
#include "modint/montgomery.hpp"

int main() {
    dR(u32, T, mod);
    if (mod == 2) {
        _for (T) {
            dR(u32, n, k);
            print(n != 0 || k != 1);
        }
        return 0;
    }
    SetMMod(u32, mod);
    _for (T) {
        dR(u32, n, k);
        print(C<mint>(n, k));
    }
    return 0;
}