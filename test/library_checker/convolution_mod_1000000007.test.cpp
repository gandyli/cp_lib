#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include "all.hpp"
#include "poly/convolution.hpp"

using mint = MMInt1000000007;
int main() {
    dR(int, n, m);
    dRV(mint, a, n);
    dRV(mint, b, m);
    print(convolution(a, b));
    return 0;
}