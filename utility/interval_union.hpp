#pragma once
#include "template.hpp"

template <typename T>
vc<std::pair<T, T>> interval_union(vc<std::pair<T, T>> a) {
    if (a.empty())
        return {};
    sort(a);
    vc<std::pair<T, T>> b;
    T l = -inf<T>, r = -inf<T>;
    foreach (x, y, a)
        if (x > r) {
            if (l != -inf<T>)
                b.eb(l, r);
            l = x, r = y;
        }
        else
            chkmax(r, y);
    b.eb(l, r);
    return b;
}