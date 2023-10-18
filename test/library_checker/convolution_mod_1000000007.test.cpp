#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include "all.hpp"
#include "ntt/arbitrary_ntt.hpp"

using Z = MMInt1000000007;
int main() {
    dR(int, n, m);
    dRV(Z, a, n);
    dRV(Z, b, m);
    writeln(ArbitraryNTT::multiply(a, b));
    return 0;
}