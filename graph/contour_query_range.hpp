#pragma once
#include "graph/centroid_decomposition.hpp"
#include "utility/generator.hpp"

template <typename G, bool WEIGHTED = false>
struct Contour_Query_Range {
    static_assert(!WEIGHTED);
    using cost_type = int;

    int n;
    vi V, comp;
    vc<cost_type> dep, comp_range{0};
    vi info_inx, info_indptr;

    Contour_Query_Range(const auto& g): n(g.n) {
        int p = 0;
        auto f = [&](auto&& par, auto&& vs, auto&& col) {
            int n = len(vs);
            vc<cost_type> dist(n);
            vi A, B;
            _for (i, 1, n)
                dist[i] = dist[par[i]] + 1;
            _for (i, n) {
                if (col[i] == 0)
                    A.eb(i);
                if (col[i] == 1)
                    B.eb(i);
            }
            if (A.empty() || B.empty())
                return;
            cost_type mx_A = 0, mx_B = 0;
            foreach (u, A)
                V.eb(vs[u]), comp.eb(p), dep.eb(dist[u]), chkmax(mx_A, dist[u]);
            comp_range.eb(comp_range.back() + mx_A + 1), p++;
            foreach (u, B)
                V.eb(vs[u]), comp.eb(p), dep.eb(dist[u]), chkmax(mx_B, dist[u]);
            comp_range.eb(comp_range.back() + mx_B + 1), p++;
        };
        centroid_decomposition<2>(g, f);
        info_indptr.assign(n + 1, 0);
        foreach (u, V)
            info_indptr[u + 1]++;
        _for (i, n)
            info_indptr[i + 1] += info_indptr[i];
        auto counter = info_indptr;
        info_inx.resize(len(V));
        _for (i, len(V))
            info_inx[counter[V[i]]++] = i;
    };
    cost_type size() const { return comp_range.back(); }
    generator<std::pair<cost_type, cost_type>> get_contour_range(int u, cost_type l, cost_type r) const {
        _for (i, info_indptr[u], info_indptr[u + 1]) {
            int idx = info_inx[i];
            int p = comp[idx] ^ 1;
            auto lo = l - dep[idx], hi = r - dep[idx];
            auto L = comp_range[p], R = comp_range[p + 1];
            chkmax(lo, 0), chkmin(hi, R - L);
            if (lo < hi)
                co_yield {L + lo, L + hi};
        }
    }
    generator<cost_type> get_indices(int u) const {
        _for (i, info_indptr[u], info_indptr[u + 1]) {
            int idx = info_inx[i];
            int p = comp[idx];
            co_yield comp_range[p] + dep[idx];
        }
    }
};