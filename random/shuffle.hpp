#pragma once
#include "base.hpp"

void Shuffle(auto first, auto last) {
    for (auto it = first; it != last; ++it)
        std::iter_swap(it, first + rnd(0, it - first + 1));
}
void Shuffle(auto&& r) { Shuffle(begin(r), end(r)); }