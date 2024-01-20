#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_large"

#include "all.hpp"
#include "poly/convolution_huge.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n, m);
    dRV(mint, a, n);
    dRV(mint, b, m);
    print(convolution_huge(a, b));
    return 0;
}