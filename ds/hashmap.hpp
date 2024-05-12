#pragma once
#include "random/hash.hpp"

template <typename K, typename V>
struct HashMapBase {
    vc<std::pair<K, V>> kv;
    vc<bool> vis;
    struct iterator {
        using difference_type = int;
        using value_type = std::pair<K, V>;
        const HashMapBase<K, V>* mp;
        int i;
        iterator() = default;
        iterator(const HashMapBase<K, V>* mp, int i): mp(mp), i(i) {}
        const value_type& operator*() const { return mp->kv[i]; }
        iterator& operator++() {
            while (++i < len(mp->kv) && !mp->vis[i]) {}
            return *this;
        }
        iterator operator++(int) {
            iterator it = *this;
            ++*this;
            return it;
        }
        bool operator==(const iterator& it) const { return i == it.i; }
    };
    iterator begin() const { return ++iterator{this, -1}; }
    iterator end() const { return {this, len(kv)}; }
    vc<K> keys() const {
        auto&& r = *this | views::keys;
        return {r.begin(), r.end()};
    }
    vc<V> values() const {
        auto&& r = *this | views::values;
        return {r.begin(), r.end()};
    }
    vc<std::pair<K, V>> items() const { return {begin(), end()}; }
};
template <typename K, typename V>
struct HashMap: HashMapBase<K, V> {
    using Base = HashMapBase<K, V>;
    using Base::kv, Base::vis;

    u32 cap, mask, shift;
    HashMap(int n = 0) { build(n); }
    void reserve(int n) {
        if (n > len(kv)) {
            HashMap<K, V> mp(n);
            _for (i, len(kv))
                if (vis[i])
                    mp[kv[i].first] = kv[i].second;
            *this = std::move(mp);
        }
    }
    V& operator[](const K& k) {
        if (!cap)
            extend();
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            kv[i] = {k, {}};
            cap--;
        }
        return kv[i].second;
    }
    V get(const K& k, V d = {}) const {
        int i = index(k);
        return vis[i] ? kv[i].second : d;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i];
    }
    void clear() { build(0); }
    int size() const { return len(kv) / 2 - cap; }

private:
    void build(int n) {
        int k = 8;
        while (k < n * 2)
            k <<= 1;
        cap = k / 2;
        mask = k - 1;
        shift = 64 - std::__lg(k);
        kv.resize(k);
        vis.assign(k, false);
    }
    void extend() { reserve(len(kv) * 2); }
    int index(const K& k) const {
        int i = hash(k) >> shift;
        while (vis[i] && kv[i].first != k)
            i = (i + 1) & mask;
        return i;
    }
};
template <typename V>
struct HashMap<char, V>: HashMapBase<char, V> {
    using K = char;
    using Base = HashMapBase<K, V>;
    using Base::kv, Base::vis;

    int sz{};
    HashMap(int = 0) { kv.resize(128), vis.resize(128); }
    void reserve(int) {}
    V& operator[](const K& k) {
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            kv[i] = {k, {}};
            sz++;
        }
        return kv[i].second;
    }
    V get(const K& k, V d = {}) const {
        int i = index(k);
        return vis[i] ? kv[i].second : d;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i];
    }
    void clear() { vis.assign(128, false), sz = 0; }
    int size() const { return sz; }
    int index(char c) { return u8(c); }
};