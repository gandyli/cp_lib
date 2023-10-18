#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "all.hpp"
#include "ntt/ntt_avx2.hpp"

using Z = MMInt998244353;
int main() {
    dR(int, n, m);
    dRV(Z, a, n);
    dRV(Z, b, m);
    writeln(NTT<Z>{}.multiply(a, b));
    return 0;
}