#pragma once
#include "ds/bitset.hpp"

vc<bitset> reachability_bitset(vc<bitset> g) {
    const int n = len(g);
    _for (i, n)
        g[i][i] = true;
    _for (k, n)
        _for (i, n)
            if (g[i][k])
                g[i] |= g[k];
    return g;
}