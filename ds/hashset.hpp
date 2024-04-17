#pragma once
#include "random/hash.hpp"

template <typename K, int LG = 20, bool KEEP_IS = false>
struct HashSet {
    static constexpr int N = 1 << LG;
    K* key;
    vi IS;
    std::bitset<N> vis;
    HashSet(): key(new K[N]) {}
    int index(const K& k) const {
        int i = hash{}(k) >> (64 - LG);
        while (vis[i] && key[i] != k)
            i = (i + 1) & (N - 1);
        return i;
    }
    bool insert(const K& k) {
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            if constexpr (KEEP_IS)
                IS.eb(i);
            return true;
        }
        return false;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i];
    }
    void clear() {
        vis.reset();
        if constexpr (KEEP_IS)
            IS.clear();
    }
    void enumerate(auto&& f) const requires KEEP_IS
    {
        foreach (i, IS)
            f(key[i]);
    }
    ~HashSet() { delete[] key; }
};