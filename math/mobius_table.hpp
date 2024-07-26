#pragma once
#include "math/zeta.hpp"

template <typename T = int>
vc<T> mobius_table(int n) {
    vc<T> mu(n + 1);
    mu[1] = 1;
    divisor_mobius(mu);
    return mu;
}