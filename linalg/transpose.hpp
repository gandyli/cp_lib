#pragma once
#include "template.hpp"

template <typename T>
vvc<T> transpose(const vvc<T>& a, int n = -1, int m = -1) {
    if (n == -1)
        n = len(a), m = len(a[0]);
    VEC(T, b, m, n);
    _for (i, n)
        _for (j, m)
            b[j][i] = a[i][j];
    return b;
}