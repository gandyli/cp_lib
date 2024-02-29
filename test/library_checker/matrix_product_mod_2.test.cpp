#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"

#include "all.hpp"
#include "ds/bitset.hpp"

int main() {
    dR(int, n, m, k);
    dRV(bitset, a, n);
    dRV(bitset, b, m);
    vc<bitset> c(n, bitset(k));
    _for (i, n)
        _for (j, m)
            if (a[i][j])
                c[i] ^= b[j];
    io.displayArray(c, '\n');
    return 0;
}