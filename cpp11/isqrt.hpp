#pragma once
#include "template.hpp"

i64 isqrt(i64 n) {
    if (n <= 0)
        return 0;
    i64 x = sqrt(n);
    while ((x + 1) * (x + 1) <= n)
        x++;
    while (x * x > n)
        x--;
    return x;
}