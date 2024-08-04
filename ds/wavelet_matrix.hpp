#pragma once
#include "ds/bit_vector.hpp"
#include "ds/index_compression.hpp"

template <typename Y, bool SMALL_Y = false>
struct Wavelet_Matrix {
    int n, m, lg;
    Index_Compression<Y, true, SMALL_Y> IDX;
    vc<Y> ItoY;
    vc<Bit_Vector> bv;
    vi pos;

    Wavelet_Matrix() = default;
    Wavelet_Matrix(const vc<Y>& a) { build(a); }
    void build(const vc<Y>& a) {
        n = len(a);
        auto [b, m] = IDX.build(a);
        this->m = m;
        ItoY.resize(m);
        _for (i, n)
            ItoY[b[i]] = a[i];
        lg = get_lg(m);
        pos.resize(lg);
        bv.assign(lg, n);
        vi b0(n), b1(n);
        _for_r (d, lg) {
            int p = 0, q = 0;
            _for (i, n) {
                if (b[i] >> d & 1)
                    b1[q++] = b[i], bv[d].set(i);
                else
                    b0[p++] = b[i];
            }
            pos[d] = p;
            bv[d].build();
            swap(b, b0);
            copy(b1.begin(), b1.begin() + q, b.begin() + p);
        }
    }
    Y kth(int l, int r, int k) const {
        int p = 0;
        _for_r (d, lg) {
            int l0 = bv[d].rank0(l), r0 = bv[d].rank0(r);
            if (k < r0 - l0)
                l = l0, r = r0;
            else
                p |= 1 << d, l += pos[d] - l0, r += pos[d] - r0, k -= r0 - l0;
        }
        return ItoY[p];
    }
    Y next(int l, int r, Y y) const {
        int k = IDX(y);
        int p = k;
        auto dfs = [&](auto&& dfs, int d, int l, int r, int x, int y) {
            if (p <= x || l == r || y <= k)
                return;
            if (!d) {
                chkmin(p, x);
                return;
            }
            d--;
            int c = (x + y) >> 1;
            int l0 = bv[d].rank0(l), r0 = bv[d].rank0(r);
            dfs(dfs, d, l0, r0, x, c);
            dfs(dfs, d, l + pos[d] - l0, r + pos[d] - r0, c, y);
        };
        dfs(dfs, lg, l, r, 0, 1 << lg);
        return p == k ? inf<Y> : ItoY[p];
    }
    Y prev(int l, int r, Y y) const {
        int k = IDX(y);
        int p = -1;
        auto dfs = [&](auto&& dfs, int d, int l, int r, int x, int y) {
            if (y - 1 <= p || l == r || k <= x)
                return;
            if (!d) {
                chkmax(p, x);
                return;
            }
            d--;
            int c = (x + y) >> 1;
            int l0 = bv[d].rank0(l), r0 = bv[d].rank0(r);
            dfs(dfs, d, l + pos[d] - l0, r + pos[d] - r0, c, y);
            dfs(dfs, d, l0, r0, x, c);
        };
        dfs(dfs, lg, l, r, 0, 1 << lg);
        return p == -1 ? -inf<Y> : ItoY[p];
    }
    Y median(int l, int r, bool upper = true) const {
        return kth(l, r, upper ? (r - l) >> 1 : (r - l - 1) >> 1);
    }
    int prefix_count(int l, int r, Y y) const {
        int p = IDX(y);
        if (l == r || p == 0)
            return 0;
        if (p == m)
            return r - l;
        int cnt = 0;
        _for_r (d, lg) {
            int l0 = bv[d].rank0(l), r0 = bv[d].rank0(r);
            if (p >> d & 1)
                cnt += r0 - l0, l += pos[d] - l0, r += pos[d] - r0;
            else
                l = l0, r = r0;
        }
        return cnt;
    }
    int count(int l, int r, Y y1, Y y2) const { return count(l, r, y2) - count(l, r, y1); }
    int freq(int l, int r, Y y) const { return count(l, r, y, y + 1); }
};