#pragma once
#include "template.hpp"

template <Integer T>
T bsearch(auto&& check, T ok, T ng) {
    while (std::abs(ok - ng) > 1) {
        T x = (ng + ok) >> 1;
        (check(x) ? ok : ng) = x;
    }
    return ok;
}
template <std::floating_point T>
T bsearch(auto&& check, T ok, T ng, int iter = 100) {
    _for (iter) {
        T x = (ng + ok) / 2;
        (check(x) ? ok : ng) = x;
    }
    return (ng + ok) / 2;
}