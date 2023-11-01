#pragma once

template <typename T>
constexpr T power(T a, auto b) {
    T ans{1};
    for (; b; b >>= 1, a *= a)
        if (b & 1)
            ans *= a;
    return ans;
}