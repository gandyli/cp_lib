#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "all.hpp"
#include "poly/ntt_avx2.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n, m);
    _for (i, n)
        io.read(_buf1[i]);
    _for (i, m)
        io.read(_buf2[i]);
    inplace_multiply<mint>(n, m, false);
    io.displayArray(_buf1, _buf1 + n + m - 1);
    return 0;
}