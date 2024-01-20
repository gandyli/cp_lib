#pragma once
#include "template.hpp"

template <typename mint>
vc<mint> all_inverse(vc<mint>& a) {
    const int n = len(a);
    vc<mint> r(n + 1);
    r[0] = 1;
    _for (i, n)
        r[i + 1] = r[i] * a[i];
    mint t = pop(r).inv();
    _for_r (i, n) {
        r[i] *= t;
        t *= a[i];
    }
    return r;
}