#pragma once
#include "template.hpp"

template <typename T>
T fixpoint(T x, auto&& f) {
    T xn = f(x);
    while (x < xn) {
        x = xn;
        xn = f(x);
    }
    while (xn < x) {
        x = xn;
        xn = f(x);
    }
    return x;
}