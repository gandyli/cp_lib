#pragma once
#include "math/factorize.hpp"

template <Unsigned T>
T primitive_root(T mod) {
    if (mod == 2)
        return 1;
    auto pf = factorize(mod - 1);
    SetMMod(T, mod);
    loop {
        T pr = rnd(2, mod);
        foreach (p, pf)
            if (power<mint>(pr, (mod - 1) / p) == 1)
                goto cont;
        return pr;
    cont:;
    }
}