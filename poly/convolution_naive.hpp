#pragma once
#include "template.hpp"

template <typename T>
vc<T> convolution_naive(const vc<T>& a, const vc<T>& b) {
    int n = len(a), m = len(b);
    if (n > m)
        return convolution_naive(b, a);
    if (!n)
        return {};
    vc<T> r(n + m - 1);
    _for (i, n)
        _for (j, m)
            r[i + j] += a[i] * b[j];
    return r;
}