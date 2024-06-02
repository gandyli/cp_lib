#pragma once
#include "template.hpp"

template <typename T = int>
struct KDTree {
    vc<std::tuple<T, T, T, T>> closed_range;
    vi dat;
    int n, lg;

    KDTree(const vc<T>& X, const vc<T>& Y): n(len(X)) {
        lg = get_lg(n);
        closed_range.resize(1 << (lg + 1));
        dat.assign(1 << (lg + 1), -1);
        vi I(n);
        iota(all(I), 0);
        build(1, X, Y, I);
    }
    vi collect_rect(T xl, T xr, T yl, T yr, int ms = -1) {
        ASSERT(xl <= xr && yl <= yr);
        if (ms == -1)
            ms = n;
        vi res;
        auto dfs = [&](auto&& dfs, int i) {
            auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
            if (xr <= xmin || xmax < xl || yr <= ymin || ymax < yl)
                return;
            if (dat[i] != -1) {
                res.eb(dat[i]);
                return;
            }
            dfs(dfs, i << 1);
            dfs(dfs, i << 1 | 1);
        };
        dfs(dfs, 1);
        return res;
    }
    template <typename U>
    auto nearest_neighbor_search(T x, T y) {
        std::pair<int, U> res{-1, -1};
        auto best_dist_squared = [&](int i, T x, T y) {
            auto& [xmin, xmax, ymin, ymax] = closed_range[i];
            U dx = x - clamp(x, xmin, xmax);
            U dy = y - clamp(y, ymin, ymax);
            return dx * dx + dy * dy;
        };
        auto dfs = [&](auto&& dfs, int i) {
            U d = best_dist_squared(i, x, y);
            if (res.first != -1 && d >= res.second)
                return;
            if (dat[i] != -1) {
                res = {dat[i], d};
                return;
            }
            U d0 = best_dist_squared(i << 1, x, y);
            U d1 = best_dist_squared(i << 1 | 1, x, y);
            if (d0 < d1)
                dfs(dfs, i << 1), dfs(dfs, i << 1 | 1);
            else
                dfs(dfs, i << 1 | 1), dfs(dfs, i << 1);
        };
        dfs(dfs, 1);
        return res;
    }

private:
    void build(int i, vi X, vi Y, vi raw_idx, bool div_x = true) {
        int n = len(X);
        auto&& [xmin, xmax, ymin, ymax] = closed_range[i];
        xmin = ymin = inf<T>;
        xmax = ymax = -inf<T>;
        _for (i, n) {
            chkmin(xmin, X[i]);
            chkmax(xmax, X[i]);
            chkmin(ymin, Y[i]);
            chkmax(ymax, Y[i]);
        }
        if (n == 1) {
            dat[i] = raw_idx[0];
            return;
        }
        int m = n >> 1;
        vi I(n);
        iota(all(I), 0);
        if (div_x)
            nth_element(I, I.begin() + m, [&](int i, int j) { return X[i] < X[j]; });
        else
            nth_element(I, I.begin() + m, [&](int i, int j) { return Y[i] < Y[j]; });
        X = rearrange(X, I), Y = rearrange(Y, I), raw_idx = rearrange(raw_idx, I);
        build(i << 1, {X.begin(), X.begin() + m}, {Y.begin(), Y.begin() + m}, {raw_idx.begin(), raw_idx.begin() + m}, !div_x);
        build(i << 1 | 1, {X.begin() + m, X.end()}, {Y.begin() + m, Y.end()}, {raw_idx.begin() + m, raw_idx.end()}, !div_x);
    }
};