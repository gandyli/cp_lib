#pragma once
#include <cstdlib>

template <typename T>
T bsearch(auto&& check, T ok, T ng) {
    while (std::abs(ok - ng) > 1) {
        T x = (ng + ok) >> 1;
        if (check(x))
            ok = x;
        else
            ng = x;
    }
    return ok;
}