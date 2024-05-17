#pragma once
#include "template.hpp"

void enumerate_combinations(auto&& a, int r, auto&& f) {
    using T = range_value_t<decltype(a)>;
    int n = len(a);
    if (r > n)
        return;
    vi I(r);
    iota(all(I), 0);
    vc<T> b(r);
    _for (i, r)
        b[i] = a[I[i]];
    f(b);
    loop {
        int i = r - 1;
        while (i >= 0 && I[i] == i + n - r)
            i--;
        if (i < 0)
            return;
        I[i]++;
        _for (j, i + 1, r)
            I[j] = I[j - 1] + 1;
        _for (i, r)
            b[i] = a[I[i]];
        f(b);
    }
}