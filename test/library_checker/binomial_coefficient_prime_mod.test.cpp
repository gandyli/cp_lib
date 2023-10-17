#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include "all.hpp"
#include "modint/binomial.hpp"
#include "modint/montgomery.hpp"

int main() {
    dR(u32, T, mod);
    if (mod == 2) {
        _for (T) {
            dR(u32, n, k);
            writeln(n != 0 || k != 1);
        }
        return 0;
    }
    using ctx = DynamicMontgomeryReductionContext<u32>;
    auto _guard = ctx::set_mod(mod);
    using Z = MontgomeryModInt<ctx>;

    Comb<Z> comb(1e7);
    
    _for (T) {
        dR(u32, n, k);
        writeln(comb(n, k));
    }
    return 0;
}