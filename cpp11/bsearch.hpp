#pragma once
#include <cstdlib>

template <typename T, typename U, typename V>
U bsearch(T&& check, U ok, V ng) {
    while (std::abs(ok - ng) > 1) {
        auto x = (ng + ok) >> 1;
        if (check(x))
            ok = x;
        else
            ng = x;
    }
    return ok;
}