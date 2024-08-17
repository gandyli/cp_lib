#pragma once
#include "template.hpp"

template <typename Monoid>
struct Wide_SegTree {
    using MX = Monoid;
    using X = MX::value_type;
    using V __attribute__((vector_size(32))) = X;

    static constexpr u32 B = 64 / sizeof(X), b = __builtin_ctz(B);
    static constexpr int calc_height(int n) { return n <= B ? 1 : calc_height(ceil(n, B)) + 1; }
    static constexpr int calc_offset(int h, int len) {
        int s = 0, n = len + 1;
        while (h--) {
            n = ceil(n, B);
            s += n * B;
        }
        return s;
    }
    static constexpr auto mask = [] {
        std::array<std::array<X, B>, B> mask;
        _for (i, B)
            _for (j, B)
                mask[i][j] = j > i ? -1 : 0;
        return mask;
    }();

    int n, height;
    vi offset;
    X *a, sum;
    Wide_SegTree() = default;
    Wide_SegTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    Wide_SegTree(const vc<T>& a) { build(a); }
    Wide_SegTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        height = calc_height(n + 1);
        offset.resize(height);
        _for (i, height)
            offset[i] = calc_offset(i, n);
        a = new (std::align_val_t(sizeof(X) * B)) X[calc_offset(height, n)]{};
        sum = MX::unit();
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        build(n);
        int id = 0;
        auto dfs = [&](auto&& dfs, int h, int cur) -> X {
            X s{};
            if (!h) {
                _for (i, B) {
                    a[cur + i] = s;
                    if (id < n)
                        s = MX::op(s, f(id));
                    id++;
                }
            }
            else {
                int nxt = (cur - offset[h]) * B + offset[h - 1];
                _for (i, B) {
                    if (id > n)
                        break;
                    a[cur + i] = s;
                    s = MX::op(s, dfs(dfs, h - 1, nxt));
                    nxt += B;
                }
            }
            return s;
        };
        sum = dfs(dfs, height - 1, offset[height - 1]);
    }
    void set(int i, const X& x) {
        multiply(i, MX::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        sum = MX::op(sum, x);
        V v{};
        v += x;
#pragma GCC unroll 16
        _for (h, height) {
            auto t = (V*)&a[offset[h] + i / B * B];
            auto m = (V*)mask[i % B].data();
            t[0] = MX::op(t[0], v & m[0]);
            t[1] = MX::op(t[1], v & m[1]);
            i >>= b;
        }
    }
    X get(int i) const { return prod(i, i + 1); }
    vc<X> get_all() const {
        vc<X> a(n);
        _for (i, n)
            a[i] = get(i);
        return a;
    }
    X prod(int i) const {
        X r = MX::unit();
#pragma GCC unroll 16
        _for (h, height)
            r = MX::op(r, a[offset[h] + (i >> (h * b))]);
        return r;
    }
    X prod(int l, int r) const { return MX::op(prod(r), MX::inverse(prod(l))); }
    X prod_all() const { return sum; }
};