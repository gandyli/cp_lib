#pragma once
#include "poly/fft.hpp"
#include "poly/convolution_naive.hpp"

template <typename R, typename U = f64, bool ROUND = false>
vc<U> convolution_fft(const vc<R>& a, const vc<R>& b) {
    using namespace CFFT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 60)
        return convolution_naive<R, U>(a, b);
    int k = get_lg(n + m - 1), sz = 1 << k;
    vc<C> c(sz);
    _for (i, n)
        c[i].x = a[i];
    _for (i, m)
        c[i].y = b[i];
    setw(k);
    fft(c, k);
    c[0].y = 4 * c[0].x * c[0].y;
    c[1].y = 4 * c[1].x * c[1].y;
    c[0].x = c[1].x = 0;
    _for (i, 2, sz, 2) {
        int j = i ^ (std::__bit_floor(i) - 1);
        c[i] = (c[i] + c[j].conj()) * (c[i] - c[j].conj());
        c[j] = -c[i].conj();
    }
    _for (i, sz >> 1) {
        C A0 = c[i << 1] + c[i << 1 | 1];
        C A1 = (c[i << 1] - c[i << 1 | 1]) * w[i].conj();
        c[i] = A0 + A1.rotl();
    }
    ifft(c, k - 1);
    vc<U> ret(n + m - 1);
    auto round = [](f64 x) { return x > 0 ? x + 0.5 : x - 0.5; };
    _for (i, n + m - 1)
        if constexpr (ROUND)
            ret[i] = round((i & 1 ? -c[i >> 1].x : c[i >> 1].y) / (4 * sz));
        else
            ret[i] = (i & 1 ? -c[i >> 1].x : c[i >> 1].y) / (4 * sz);
    return ret;
}
