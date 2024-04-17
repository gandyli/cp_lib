#pragma once
#include "ds/segtree_beats.hpp"

template <typename T>
struct Beats_SumMax_AddChmin {
    struct SumMax {
        struct X {
            T sum, max, maxc, max2;
            bool fail;
        };
        using value_type = X;
        static constexpr X op(const X& a, const X& b) {
            if (a.max == -inf<T>)
                return b;
            if (b.max == -inf<T>)
                return a;
            X r;
            r.sum = a.sum + b.sum;
            r.max = Max(a.max, b.max);
            r.maxc = a.max == b.max ? a.maxc + b.maxc : (a.max > b.max ? a.maxc : b.maxc);
            r.max2 = -inf<T>;
            if (r.max > a.max)
                chkmax(r.max2, a.max);
            else if (r.max > a.max2)
                chkmax(r.max2, a.max2);
            if (r.max > b.max)
                chkmax(r.max2, b.max);
            else if (r.max > b.max2)
                chkmax(r.max2, b.max2);
            r.fail = false;
            return r;
        }
        static constexpr X from_element(auto&& x) { return x; }
        static constexpr X unit() { return {0, -inf<T>, 0, -inf<T>}; }
        static constexpr bool commute() { return true; }
    };
    struct AddChmin {
        using value_type = std::pair<T, T>;
        using X = value_type;
        static constexpr X op(const X& a, const X& b) { return {a.first + b.first, Min(a.second + b.first, b.second)}; }
        static constexpr X from_element(auto&& x) { return x; }
        static constexpr X unit() { return {0, inf<T>}; }
        static constexpr bool commute() { return false; }
    };
    struct Beats {
        using Monoid_X = SumMax;
        using Monoid_A = AddChmin;
        using X = Monoid_X::value_type;
        using A = Monoid_A::value_type;
        static constexpr X act(X x, const A& a, int sz) {
            ASSERT(!x.fail);
            if (x.max == -inf<T>)
                return x;
            auto&& [add, chmin] = a;
            x.sum += add * sz;
            x.max += add;
            x.max2 += add;
            if (chmin == inf<T>)
                return x;
            T t = x.max;
            chkmin(x.max, chmin);
            if (x.maxc == sz) {
                x.max2 = x.max;
                x.sum = x.max * sz;
            }
            else if (x.max2 < x.max)
                x.sum += (x.max - t) * x.maxc;
            else
                x.fail = true;
            return x;
        }
    };
    struct X {
        T sum, max;
        static X from_element(const Beats::X& x) { return {x.sum, x.max}; }
    };
    SegTree_Beats<Beats> seg;
    int n;
    Beats_SumMax_AddChmin(const vc<T>& a) { build(a); }
    Beats_SumMax_AddChmin(int n, std::invocable<int> auto&& f) { build(n, f); }
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        seg.build(n, [&](int i) { return from_element(f(i)); });
    }
    void set(int i, T x) { seg.set(i, from_element(x)); }
    X get(int i) { return X::from_element(seg.get(i)); }
    vc<X> get_all() {
        vc<X> r(n);
        auto t = seg.get_all();
        _for (i, n)
            r[i] = X::from_element(t[i]);
        return r;
    }
    X prod(int l, int r) { return X::from_element(seg.prod(l, r)); }
    X prod_all() { return X::from_element(seg.prod_all()); }
    static Beats::X from_element(T x) { return {x, x, 1, x}; }
    void chmin(int l, int r, T x) { seg.apply(l, r, {0, x}); }
    void add(int l, int r, T x) { seg.apply(l, r, {x, inf<T>}); }
};