#pragma once
#include "ds/bit_vector.hpp"

template <typename T, bool COMPRESS = true, int LG = -1>
struct Wavelet_Matrix {
    using X = std::conditional_t<COMPRESS, int, T>;
    int n, lg;
    vc<Bit_Vector> b;
    vi pos;
    vc<T> c;
    Wavelet_Matrix() = default;
    Wavelet_Matrix(const vc<T>& a) { build(a); }
    void build0(vc<X> a) {
        pos.resize(lg);
        b.assign(lg, n);
        vc<X> a0(n), a1(n);
        _for_r (d, lg) {
            int p = 0, q = 0;
            _for (i, n) {
                if (a[i] >> d & 1)
                    a1[q++] = a[i], b[d].set(i);
                else
                    a0[p++] = a[i];
            }
            pos[d] = p;
            b[d].build();
            swap(a, a0);
            copy(a1.begin(), a1.begin() + q, a.begin() + p);
        }
    }
    void build(const vc<T>& a) {
        n = len(a);
        if constexpr (COMPRESS) {
            c.reserve(n);
            vi d(n);
            foreach (i, argsort(a)) {
                if (c.empty() || c.back() != a[i])
                    c.eb(a[i]);
                d[i] = len(c) - 1;
            }
            c.shrink_to_fit();
            lg = std::bit_width<u64>(Max(len(c) - 1, 1));
            build0(std::move(d));
        }
        else {
            if constexpr (LG == -1)
                lg = std::bit_width<u64>(Max(max(a), 1));
            else
                lg = LG;
            build0(a);
        }
    }
    T kth(int l, int r, int k) const {
        X res = 0;
        _for_r (d, lg) {
            int l0 = b[d].rank0(l), r0 = b[d].rank0(r);
            if (k < r0 - l0)
                l = l0, r = r0;
            else
                res |= X(1) << d, l += pos[d] - l0, r += pos[d] - r0, k -= r0 - l0;
        }
        if constexpr (COMPRESS)
            return c[res];
        return res;
    }
    int count(int l, int r, T x) const {
        if constexpr (COMPRESS)
            x = LB(c, x);
        if (x == 0)
            return 0;
        if (x >= X(1) << lg)
            return r - l;
        int cnt = 0;
        _for_r (d, lg) {
            int l0 = b[d].rank0(l), r0 = b[d].rank0(r);
            if (x >> d & 1)
                cnt += r0 - l0, l += pos[d] - l0, r += pos[d] - r0;
            else
                l = l0, r = r0;
        }
        return cnt;
    }
    int count(int l, int r, T x, T y) const { return count(l, r, y) - count(l, r, x); }
    int freq(int l, int r, T x) const { return count(l, r, x, x + 1); }
};