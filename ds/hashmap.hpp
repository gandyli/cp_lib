#pragma once
#include "random/hash.hpp"

template <typename K, typename V>
struct HashMap {
    u32 cap, mask, shift;
    vc<K> key;
    vc<V> val;
    vc<bool> vis;
    HashMap(int n = 0) { build(n); }
    void build(int n) {
        int k = 8;
        while (k < n * 2)
            k <<= 1;
        cap = k / 2;
        mask = k - 1;
        shift = 64 - std::__lg(k);
        key.resize(k);
        val.resize(k);
        vis.assign(k, false);
    }
    void extend() {
        HashMap<K, V> hm(len(key) * 2);
        _for (i, len(key))
            if (vis[i])
                hm[key[i]] = val[i];
        *this = std::move(hm);
    }
    int index(const K& k) const {
        int i = hash(k) >> shift;
        while (vis[i] && key[i] != k)
            i = (i + 1) & mask;
        return i;
    }
    V& operator[](const K& k) {
        if (!cap)
            extend();
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            val[i] = {};
            cap--;
        }
        return val[i];
    }
    V get(const K& k, V d = {}) const {
        int i = index(k);
        return vis[i] ? val[i] : d;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i];
    }
    void clear() { build(0); }
    int size() const { return len(key) - cap; }
    void enumerate(auto&& f) const {
        _for (i, len(vis))
            if (vis[i])
                f(key[i], val[i]);
    }
};