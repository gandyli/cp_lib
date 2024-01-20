#pragma once
#include "poly/convolution.hpp"

template <Modint mint>
vc<mint> convolution_huge(const vc<mint>& a, const vc<mint>& b) {
    constexpr NTT<mint> ntt;
    constexpr int k = ntt.lvl;
    constexpr int mask = (1 << (k - 1)) - 1;
    constexpr mint iv = mint(1 << k).inv();
    int n = len(a), m = len(b);
    if (n + m - 1 <= (1 << k))
        return convolution(a, b);
    VEC(mint, c, 4, 1 << k);
    VEC(mint, d, 4, 1 << k);
    _for (i, n)
        c[i >> (k - 1)][i & mask] = a[i];
    _for (i, m)
        d[i >> (k - 1)][i & mask] = b[i];
    _for (i, 4)
        fft4(c[i], k);
    _for (i, 4)
        fft4(d[i], k);
    vc<mint> ans(4 << k);
    _for (i, 7) {
        vc<mint> E(1 << k);
        _for (t, 4)
            if (0 <= i - t && i - t < 4)
                _for (j, 1 << k)
                    E[j] += c[t][j] * d[i - t][j];
        ifft4(E, k);
        _for (j, 1 << k)
            ans[(i << (k - 1)) + j] += E[j] * iv;
    }
    ans.resize(n + m - 1);
    return ans;
}
