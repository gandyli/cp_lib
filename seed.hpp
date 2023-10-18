#pragma once
#include "template.hpp"

namespace seed_impl {
    struct Seed {
        u64 seed;
        Seed() {
#if defined(LX_LOCAL) && !defined(RANDOM_SEED)
            seed = 25252336454627188;
#else
            seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            seed ^= 5624630172374385489;
            seed ^= seed << 24, seed ^= seed >> 31, seed ^= seed << 35;
#endif
        }
    } seed;
} // namespace seed_impl
u64& seed() { return seed_impl::seed.seed; }