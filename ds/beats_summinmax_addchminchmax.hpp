#pragma once
#include "ds/segtree_beats.hpp"

template <typename T>
struct Beats_SumMinMax_AddChminChmax {
    struct SumMinMax {
        struct X {
            T sum, min, minc, min2, max, maxc, max2;
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
            r.min = Min(a.min, b.min), r.max = Max(a.max, b.max);
            r.minc = a.min == b.min ? a.minc + b.minc : (a.min < b.min ? a.minc : b.minc);
            r.min2 = r.max;
            if (r.min < a.min)
                chkmin(r.min2, a.min);
            else if (r.min < a.min2)
                chkmin(r.min2, a.min2);
            if (r.min < b.min)
                chkmin(r.min2, b.min);
            else if (r.min < b.min2)
                chkmin(r.min2, b.min2);
            r.maxc = a.max == b.max ? a.maxc + b.maxc : (a.max > b.max ? a.maxc : b.maxc);
            r.max2 = r.min;
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
        static constexpr X unit() { return {0, inf<T>, 0, inf<T>, -inf<T>, 0, -inf<T>}; }
        static constexpr bool commute() { return true; }
    };
    struct AddChminChmax {
        using value_type = std::tuple<T, T, T>;
        using X = value_type;
        static constexpr X op(const X& a, const X& b) {
            auto&& [add, chmin, chmax] = a;
            auto&& [add2, chmin2, chmax2] = b;
            return {add + add2, Min(chmin + add2, chmin2), Max(Min(chmax + add2, chmin2), chmax2)};
        }
        static constexpr X from_element(auto&& x) { return x; }
        static constexpr X unit() { return {0, inf<T>, -inf<T>}; }
        static constexpr bool commute() { return false; }
    };
    struct Beats {
        using Monoid_X = SumMinMax;
        using Monoid_A = AddChminChmax;
        using X = Monoid_X::value_type;
        using A = Monoid_A::value_type;
        static constexpr X act(X x, const A& a, int sz) {
            ASSERT(!x.fail);
            if (x.min == inf<T>)
                return x;
            auto&& [add, chmin, chmax] = a;
            x.sum += add * sz;
            x.min += add;
            x.min2 += add;
            x.max += add;
            x.max2 += add;
            if (chmin == inf<T> && chmax == -inf<T>)
                return x;
            T tmin = x.min, tmax = x.max;
            chkmin(x.min, chmin);
            chkmax(x.min, chmax);
            chkmin(x.max, chmin);
            chkmax(x.max, chmax);
            if (x.min == x.max) {
                x.sum = x.max * sz;
                x.minc = x.maxc = sz;
                x.min2 = x.max2 = x.max;
            }
            else if (x.max2 <= x.min) {
                x.min2 = x.max, x.max2 = x.min;
                x.minc = sz - x.maxc;
                x.sum = x.max * x.maxc + x.min * x.minc;
            }
            else if (x.min2 >= x.max) {
                x.min2 = x.max, x.max2 = x.min;
                x.maxc = sz - x.minc;
                x.sum = x.min * x.minc + x.max * x.maxc;
            }
            else if (x.min < x.min2 && x.max > x.max2)
                x.sum += (x.min - tmin) * x.minc + (x.max - tmax) * x.maxc;
            else
                x.fail = true;
            return x;
        }
    };
    struct X {
        T sum, max, min;
        static X from_element(const Beats::X& x) { return {x.sum, x.max, x.min}; }
    };
    SegTree_Beats<Beats> seg;
    int n;
    Beats_SumMinMax_AddChminChmax(const vc<T>& a) { build(a); }
    Beats_SumMinMax_AddChminChmax(int n, std::invocable<int> auto&& f) { build(n, f); }
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
    static Beats::X from_element(T x) { return {x, x, 1, x, x, 1, x}; }
    void chmin(int l, int r, T x) { seg.apply(l, r, {0, x, -inf<T>}); }
    void chmax(int l, int r, T x) { seg.apply(l, r, {0, inf<T>, x}); }
    void add(int l, int r, T x) { seg.apply(l, r, {x, inf<T>, -inf<T>}); }
};