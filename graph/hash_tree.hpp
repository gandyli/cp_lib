#pragma once
#include "random/hash.hpp"

u64 hash_tree(const auto& g, int r = 0) {
    auto dfs = [&](auto&& dfs, int u, int f) -> u64 {
        u64 h = 1;
        foreach (v, g[u])
            if (v != f)
                h += hash(dfs(dfs, v, u));
        return h;
    };
    return dfs(dfs, r, -1);
}