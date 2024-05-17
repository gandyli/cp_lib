#pragma once
#include "template.hpp"

void enumerate_permutations(auto&& a, int r, auto&& f) {
    using T = range_value_t<decltype(a)>;
    int n = len(a);
    if (r > n)
        return;
    vi I(n);
    iota(all(I), 0);
    vi C(r);
    _for (i, r)
        C[i] = n - i;
    vc<T> b(r);
    _for (i, r)
        b[i] = a[I[i]];
    f(b);
    loop {
        bool done = true;
        _for_r (i, r) {
            if (!--C[i]) {
                rotate(I.begin() + i, I.begin() + i + 1, I.end());
                C[i] = n - i;
            }
            else {
                int j = C[i];
                swap(I[i], I[n - j]);
                _for (i, r)
                    b[i] = a[I[i]];
                f(b);
                done = false;
                break;
            }
        }
        if (done)
            return;
    }
}