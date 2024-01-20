#pragma once
#include "ds/segtree_beats.hpp"

template <typename T>
struct Beats_SumMin_AddChmax {
    struct SumMin {
        struct X {
            T sum, min, minc, min2;
            bool fail;
        };
        using value_type = X;
        static constexpr X op(const X& a, const X& b) {
            if (a.min == inf<T>)
                return b;
            if (b.min == inf<T>)
                return a;
            X r;
            r.sum = a.sum + b.sum;
            r.min = Min(a.min, b.min);
            r.minc = a.min == b.min ? a.minc + b.minc : (a.min < b.min ? a.minc : b.minc);
            r.min2 = inf<T>;
            if (r.min < a.min)
                chkmin(r.min2, a.min);
            else if (r.min < a.min2)
                chkmin(r.min2, a.min2);
            if (r.min < b.min)
                chkmin(r.min2, b.min);
            else if (r.min < b.min2)
                chkmin(r.min2, b.min2);
            r.fail = false;
            return r;
        }
        static constexpr X from_element(auto&& x) { return x; }
        static constexpr X unit() { return {0, inf<T>, 0, inf<T>}; }
        static constexpr bool commute = true;
    };
    struct AddChmax {
        using value_type = std::pair<T, T>;
        using X = value_type;
        static constexpr X op(const X& a, const X& b) { return {a.first + b.first, Max(a.second + b.first, b.second)}; }
        static constexpr X from_element(auto&& x) { return x; }
        static constexpr X unit() { return {0, -inf<T>}; }
        static constexpr bool commute = false;
    };
    struct Beats {
        using Monoid_X = SumMin;
        using Monoid_A = AddChmax;
        using X = Monoid_X::value_type;
        using A = Monoid_A::value_type;
        static constexpr X act(X x, const A& a, int sz) {
            ASSERT(!x.fail);
            if (x.min == inf<T>)
                return x;
            auto&& [add, chmax] = a;
            x.sum += add * sz;
            x.min += add;
            x.min2 += add;
            if (chmax == -inf<T>)
                return x;
            T t = x.min;
            chkmax(x.min, chmax);
            if (x.minc == sz) {
                x.min2 = x.min;
                x.sum = x.min * sz;
            }
            else if (x.min2 > x.min)
                x.sum += (x.min - t) * x.minc;
            else
                x.fail = true;
            return x;
        }
    };
    struct X {
        T sum, min;
        static X from_element(const Beats::X& x) { return {x.sum, x.min}; }
    };
    SegTree_Beats<Beats> seg;
    int n;
    Beats_SumMin_AddChmax(const vc<T>& a) { build(a); }
    Beats_SumMin_AddChmax(int n, std::invocable<int> auto&& f) { build(n, f); }
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
    void chmax(int l, int r, T x) { seg.apply(l, r, {0, x}); }
    void add(int l, int r, T x) { seg.apply(l, r, {x, -inf<T>}); }
};