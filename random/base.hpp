#pragma once
#include "seed.hpp"

u64 rnd64() {
    u64& s = seed();
    return s ^= s << 7, s ^= s >> 9;
}
i64 rnd(i64 l, i64 r) {
    return l + rnd64() % (r - l);
}
u32 rnd() { return rnd64(); }