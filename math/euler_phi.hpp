#include "math/factorize.hpp"

template <Unsigned T>
T euler_phi(T n) {
    for (auto&& [p, _]: factorize_pair(n))
        n -= n / p;
    return n;
}