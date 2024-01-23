#pragma once
#include "math/isqrt.hpp"
#include "math/icbrt.hpp"

template <Unsigned T>
T inth_root(T a, T n) {
    ASSERT(n);
    if (n == 1)
        return a;
    if (n == 2)
        return isqrt(a);
    if (n == 3)
        return icbrt(a);
    if (std::numeric_limits<T>::digits <= n || a < (T(1) << n))
        return a > 0;
    auto guess = [&](T x) -> T {
        if (x <= u32(-1))
            return T(1) << ceil(std::__lg(x), n);
        return exp(log(x) / n) + 1;
    };
    T n1 = n - 1;
    auto next = [&](T x) {
        auto checked_pow = [&](T a, T n) -> T {
            if (a == 0)
                return 0;
            T r = 1;
            while (n) {
                if (n & 1) {
                    if (r > std::numeric_limits<T>::max() / a)
                        return 0;
                    r *= a;
                }
                n >>= 1;
                if (n) {
                    if (a > std::numeric_limits<T>::max() / a)
                        return 0;
                    a *= a;
                }
            }
            return r;
        };
        T y = checked_pow(x, n1);
        if (y)
            y = a / y;
        return (y + x * n1) / n;
    };
    return fixpoint(guess(a), next);
}
template <Signed T>
T inth_root(T a, T n) {
    if (a >= 0)
        return inth_root<std::make_signed_t<T>>(a, n);
    ASSERT(n & 1);
    return -inth_root<std::make_signed_t<T>>(-a, n);
}