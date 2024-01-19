#pragma once
#include "random/hash.hpp"

template <typename K, typename V, int LG = 20, bool KEEP_IS = false>
struct HashMap {
    static constexpr int N = 1 << LG;
    K* key = new K[N];
    V* val = new V[N];
    vi IS;
    std::bitset<N> vis;
    int index(const K& k) const {
        int i = hash{}(k) >> (64 - LG);
        while (vis[i] && key[i] != k)
            i = (i + 1) & (N - 1);
        return i;
    }
    V& operator[](const K& k) {
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            val[i] = {};
            if constexpr (KEEP_IS)
                IS.eb(i);
        }
        return val[i];
    }
    V get(const K& k, V d = {}) const {
        int i = index(k);
        return vis[i] ? val[i] : d;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i] && key[i] == k;
    }
    void clear() {
        vis.reset();
        if constexpr (KEEP_IS)
            IS.clear();
    }
    void enumerate(auto&& f) const requires KEEP_IS
    {
        foreach (i, IS)
            f(key[i], val[i]);
    }
    ~HashMap() {
        delete[] key;
        delete[] val;
    }
};