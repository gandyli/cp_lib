#pragma once
#include "template.hpp"

template <typename T, typename U = T>
vc<U> convolution_naive(const vc<T>& a, const vc<T>& b) {
    int n = len(a), m = len(b);
    if (n > m)
        return convolution_naive<T, U>(b, a);
    if (!n)
        return {};
    vc<U> r(n + m - 1);
    _for (i, n)
        _for (j, m)
            r[i + j] += U(a[i]) * b[j];
    return r;
}