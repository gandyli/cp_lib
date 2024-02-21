#pragma once
#include "poly/convolution_naive.hpp"

template <typename T, typename U = T>
vc<U> convolution_karatsuba(const vc<T>& f, const vc<T>& g) {
    int n = len(f), m = len(g);
    if (min(n, m) <= 30)
        return convolution_naive<T, U>(f, g);
    int mi = ceil(max(n, m), 2);
    vc<T> f1, f2, g1, g2;
    if (n < mi)
        f1 = f;
    else {
        f1 = {f.begin(), f.begin() + mi};
        f2 = {f.begin() + mi, f.end()};
    }
    if (m < mi)
        g1 = g;
    else {
        g1 = {g.begin(), g.begin() + mi};
        g2 = {g.begin() + mi, g.end()};
    }
    auto a = convolution_karatsuba<T, U>(f1, g1);
    auto b = convolution_karatsuba<T, U>(f2, g2);
    _for (i, len(f2))
        f1[i] += f2[i];
    _for (i, len(g2))
        g1[i] += g2[i];
    auto c = convolution_karatsuba<T, U>(f1, g1);
    vc<U> r(n + m - 1);
    _for (i, len(a))
        r[i] += a[i], c[i] -= a[i];
    _for (i, len(b))
        r[2 * mi + i] += b[i], c[i] -= b[i];
    if (c.back() == 0)
        c.pop_back();
    _for (i, len(c))
        r[mi + i] += c[i];
    return r;
}