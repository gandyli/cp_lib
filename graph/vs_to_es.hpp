#pragma once
#include "graph/base.hpp"
#include "ds/hashmap.hpp"

template <bool allow_twice = false>
vi vs_to_es(const auto& g, const vi& vs) {
    const int n = g.n, m = g.m;
    ASSERT(!vs.empty());

    static HashMap<u64, int> mp;
    mp.clear();
    vi nxt(m, -1);
    auto get = [&](int a, int b) {
        if (g.is_directed() && a > b)
            swap(a, b);
        return u64(a) * n + b;
    };
    _for (i, m) {
        u64 k = get(g.edges[i].from, g.edges[i].to);
        nxt[i] = std::exchange(mp[k], i);
    }
    vi es(len(vs) - 1);
    _for (i, len(es)) {
        u64 k = get(vs[i], vs[i + 1]);
        int e = mp.get(k, -1);
        ASSERT(e != -1);
        es[i] = e;
        if constexpr (!allow_twice)
            mp[k] = nxt[e];
    }
    return es;
}