#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "all.hpp"
#include "poly/ntt_avx2.hpp"
#include "poly/convolution.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n, m);
    dRV(mint, a, n);
    dRV(mint, b, m);
    print(convolution(a, b));
    return 0;
}