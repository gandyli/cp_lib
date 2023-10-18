#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "all.hpp"
#include "ntt/ntt_avx2.hpp"

using Z = MMInt998244353;
int main() {
    dR(int, n, m);
    NTT<Z> ntt;
    _for (i, n)
        io.read(NTT<Z>::_buf1[i]);
    _for (i, m)
        io.read(NTT<Z>::_buf2[i]);
    ntt.inplace_multiply(n, m, false);
    io.displayArray(NTT<Z>::_buf1, NTT<Z>::_buf1 + n + m - 1);
    return 0;
}