#pragma once
#include "template.hpp"

template <typename V>
vc<V> transpose(const vc<V>& a) {
    int n = len(a), m = len(a[0]);
    vc<V> b(m, V(n, decltype(a[0][0]){}));
    _for (i, n)
        _for (j, m)
            b[j][i] = a[i][j];
    return b;
}