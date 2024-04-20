#pragma once
#include "ds/fenwicktree.hpp"
#include "monoid/add.hpp"
#include "math/binary_search.hpp"

template <template <typename> typename FenwickTree>
struct FenwickTree_01Base {
    int n, m;
    vc<u64> dat;
    FenwickTree<Monoid_Add<int>> ft;
    FenwickTree_01Base() = default;
    FenwickTree_01Base(int n) { build(n); }
    template <std::convertible_to<bool> T>
    FenwickTree_01Base(const vc<T>& a) { build(a); }
    FenwickTree_01Base(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        m = (n >> 6) + 1;
        dat.assign(m, 0);
        ft.build(m);
    }
    template <std::convertible_to<bool> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        m = (n >> 6) + 1;
        dat.assign(m, 0);
        _for (i, n)
            dat[i >> 6] |= u64(f(i)) << (i & 63);
        ft.build(m, [&](int i) { return std::popcount(dat[i]); });
    }
    void add(int i) {
        dat[i >> 6] |= 1ULL << (i & 63);
        ft.multiply(i >> 6, 1);
    }
    void remove(int i) {
        dat[i >> 6] &= ~(1ULL << (i & 63));
        ft.multiply(i >> 6, -1);
    }
    void multiply(int i, int x) {
        if (x == 1)
            add(i);
        else
            remove(i);
    }
    bool get(int i) const { return dat[i >> 6] >> (i & 63) & 1; }
    vc<bool> get_all() const {
        vc<bool> a(n);
        _for (i, n)
            a[i] = get(i);
        return a;
    }
    int prod(int i) const {
        int ans = ft.prod(i >> 6);
        ans += std::popcount(dat[i >> 6] & ((1ULL << (i & 63)) - 1));
        return ans;
    }
    int prod(int l, int r) const {
        int ans = ft.prod(l >> 6, r >> 6);
        ans += std::popcount(dat[r >> 6] & ((1ULL << (r & 63)) - 1));
        ans -= std::popcount(dat[l >> 6] & ((1ULL << (l & 63)) - 1));
        return ans;
    }
    int prod_all() const { return ft.prod_all(); }
    int kth(int k, int l = 0) const {
        if (k >= prod_all())
            return n;
        k += std::popcount(dat[l >> 6] & ((1ULL << (l & 63)) - 1));
        l >>= 6;
        int mi = 0;
        int i = ft.max_right([&](int x) { return x <= k ? (chkmax(mi, x), true) : false; }, l);
        if (i == m)
            return n;
        k -= mi;
        u64 x = dat[i];
        int p = std::popcount(x);
        if (p <= k)
            return n;
        k = bsearch([&](int mi) { return (p - std::popcount(x >> mi)) <= k; }, 0, 64);
        return 64 * i + k;
    }
    int next(int i) const {
        int j = i >> 6;
        i &= 63;
        u64 x = dat[j] & ~((1ULL << i) - 1);
        if (x)
            return 64 * j + __builtin_ctzll(x);
        j = ft.kth(0, j + 1);
        if (j == m || !dat[j])
            return n;
        return 64 * j + __builtin_ctzll(dat[j]);
    }
    int prev(int i) const {
        if (i == n)
            i--;
        int j = i >> 6;
        i &= 63;
        u64 x = dat[j];
        if (i < 63)
            x &= (1ULL << (i + 1)) - 1;
        if (x)
            return 64 * j + 63 - __builtin_clzll(x);
        j = ft.min_left([&](int x) { return x <= 0; }, j) - 1;
        if (j == -1)
            return -1;
        return 64 * j + 63 - __builtin_clzll(dat[j]);
    }
};
using FenwickTree_01 = FenwickTree_01Base<FenwickTree>;