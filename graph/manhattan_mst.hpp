#pragma once
#include "graph/base.hpp"
#include "ds/unionfind.hpp"

template <typename T>
Graph<T> manhattan_mst(vc<std::pair<T, T>>& a) {
    const int n = len(a);
    vc<std::tuple<T, int, int>> dat;
    dat.reserve(4 * n);
    vi I(n);
    iota(all(I), 0);
    _for (2) {
        foreach (x, y, a)
            x = -x;
        _for (2) {
            foreach (x, y, a)
                swap(x, y);
            sort(I, [&](int i, int j) {
                return a[i].first + a[i].second < a[j].first + a[j].second;
            });
            std::map<T, int> mp;
            foreach (i, I) {
                auto&& [x, y] = a[i];
                for (auto it = mp.lower_bound(-y); it != mp.end(); it = mp.erase(it)) {
                    int j = it->second;
                    auto&& [x0, y0] = a[j];
                    int dx = x - x0, dy = y - y0;
                    if (dy > dx)
                        break;
                    dat.eb(dx + dy, i, j);
                }
                mp[-y] = i;
            }
        }
    }
    sort(dat, [&](auto&& a, auto&& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    Graph<T> mst(n);
    UnionFind uf(n);
    foreach (c, i, j, dat)
        if (uf.merge(i, j))
            mst.add(i, j, c);
    mst.build();
    return mst;
}