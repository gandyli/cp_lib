#pragma once
#include <cstdlib>

auto bsearch(auto&& check, auto ok, auto ng) {
    while (std::abs(ok - ng) > 1) {
        auto x = (ng + ok) >> 1;
        if (check(x))
            ok = x;
        else
            ng = x;
    }
    return ok;
}