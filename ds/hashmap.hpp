#pragma once
#include "random/hash.hpp"

template <typename K, typename V>
struct HashMapBase {
    vc<K> key;
    vc<V> val;
    vc<bool> vis;
    struct iterator {
        using difference_type = int;
        using value_type = std::pair<K, V>;
        const HashMapBase<K, V>* mp;
        int i;
        iterator() = default;
        iterator(const HashMapBase<K, V>* mp, int i): mp(mp), i(i) {}
        value_type operator*() const { return {mp->key[i], mp->val[i]}; }
        iterator& operator++() {
            while (++i < len(mp->key) && !mp->vis[i]) {}
            return *this;
        }
        iterator operator++(int) {
            iterator it = *this;
            ++*this;
            return it;
        }
        bool operator==(const iterator& it) const { return i == it.i; }
    };
    iterator begin() const {
        int i = 0;
        while (i < len(key) && !vis[i])
            i++;
        return {this, i};
    }
    iterator end() const { return {this, len(key)}; }
    vc<K> keys() const {
        vc<K> res;
        foreach (k, v, *this)
            res.eb(k);
        return res;
    }
    vc<V> values() const {
        vc<V> res;
        foreach (k, v, *this)
            res.eb(v);
        return res;
    }
    vc<std::pair<K, V>> items() const {
        vc<std::pair<K, V>> res;
        foreach (k, v, *this)
            res.eb(k, v);
        return res;
    }
};
template <typename K, typename V>
struct HashMap: HashMapBase<K, V> {
    using Base = HashMapBase<K, V>;
    using Base::key, Base::val, Base::vis;

    u32 cap, mask, shift;
    HashMap(int n = 0) { build(n); }
    void reserve(int n) {
        if (n > len(key)) {
            HashMap<K, V> mp(n);
            _for (i, len(key))
                if (vis[i])
                    mp[key[i]] = val[i];
            *this = std::move(mp);
        }
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
    int size() const { return len(key) / 2 - cap; }

private:
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
    void extend() { reserve(len(key) * 2); }
    int index(const K& k) const {
        int i = hash(k) >> shift;
        while (vis[i] && key[i] != k)
            i = (i + 1) & mask;
        return i;
    }
};
template <typename V>
struct HashMap<char, V>: HashMapBase<char, V> {
    using K = char;
    using Base = HashMapBase<K, V>;
    using Base::key, Base::val, Base::vis;

    int sz{};
    HashMap(int = 0) { key.resize(128), val.resize(128), vis.resize(128); }
    void reserve(int) {}
    V& operator[](const K& k) {
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            val[i] = {};
            sz++;
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
    void clear() { vis.assign(128, false), sz = 0; }
    int size() const { return sz; }
    int index(char c) { return u8(c); }
};