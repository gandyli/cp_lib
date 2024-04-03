#pragma once
#include "math/factorize.hpp"

template <Unsigned T>
vc<T> divisors(T n) {
    vc<T> d{1};
    foreach (p, c, factorize_pair(n)) {
        int t = len(d);
        T pp = 1;
        _for (c) {
            pp *= p;
            _for (i, t)
                d.eb(d[i] * pp);
        }
    }
    return d;
}
vi divisors(int n, const vi& lpf) {
    vi d{1};
    foreach (p, c, factorize_pair(n, lpf)) {
        int t = len(d);
        int pp = 1;
        _for (c) {
            pp *= p;
            _for (i, t)
                d.eb(d[i] * pp);
        }
    }
    return d;
}