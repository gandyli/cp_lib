#pragma once
#include "graph/base.hpp"

template <typename G, bool VTOE = false>
struct Centroid_Decomposition {
    static std::pair<vi, int> find_centroid(auto& par) {
        const int n = len(par);
        vi sz(n, 1);
        _for_r (i, n) {
            if (sz[i] >= ceil(n, 2))
                return {std::move(sz), i};
            sz[par[i]] += sz[i];
        }
        return {};
    }
    static void dfs0(vi& par, vi& vs, auto&& f) {
        const int n = len(vs);
        ASSERT(n >= 1);
        auto c = find_centroid(par).second;
        vi col(n);
        vi V{c};
        int nc = 1;
        _for (u, 1, n) {
            if (par[u] == c) {
                V.eb(u);
                col[u] = nc++;
            }
        }
        if (c > 0) {
            for (int u = par[c]; u != -1; u = par[u]) {
                V.eb(u);
                col[u] = nc;
            }
            nc++;
        }
        _for (u, n) {
            if (u != c && !col[u]) {
                V.eb(u);
                col[u] = col[par[u]];
            }
        }
        vi indptr(nc + 1);
        _for (i, n)
            indptr[col[i] + 1]++;
        _for (i, nc)
            indptr[i + 1] += indptr[i];
        auto counter = indptr;
        vi ord(n);
        foreach (u, V)
            ord[counter[col[u]]++] = u;
        vi new_idx(n);
        _for (i, n)
            new_idx[ord[i]] = i;
        vi name(n);
        _for (i, n)
            name[new_idx[i]] = vs[i];
        {
            vi tmp(n, -1);
            _for (i, 1, n) {
                int a = new_idx[i], b = new_idx[par[i]];
                if (a > b)
                    swap(a, b);
                tmp[b] = a;
            }
            par = std::move(tmp);
        }
        f(par, name, indptr);
        _for (i, 1, nc) {
            int l = indptr[i], r = indptr[i + 1];
            vi par1(r - l);
            vi name1(r - l);
            _for (i, l, r)
                name1[i - l] = name[i];
            _for (i, l, r)
                par1[i - l] = max(par[i] - l, -1);
            dfs0(par1, name1, f);
        }
    }
    static void dfs1(vi& par, vi& vs, auto&& f) {
        const int n = len(vs);
        ASSERT(n > 1);
        if (n == 2)
            return;
        auto [sz, c] = find_centroid(par);
        vi col(n, -1);
        int take = 0;
        vi ord(n, -1);
        ord[c] = 0;
        int p = 1;
        _for (u, 1, n) {
            if (par[u] == c && take + sz[u] <= floor(n - 1, 2)) {
                ord[u] = p++;
                col[u] = 0;
                take += sz[u];
            }
        }
        _for (u, 1, n)
            if (!col[par[u]]) {
                ord[u] = p++;
                col[u] = 0;
            }
        int n0 = p - 1;
        for (int u = par[c]; u != -1; u = par[u]) {
            ord[u] = p++;
            col[u] = 1;
        }
        _for (u, n)
            if (u != c && ord[u] == -1) {
                ord[u] = p++;
                col[u] = 1;
            }
        ASSERT(p == n);
        int n1 = n - 1 - n0;
        vi par0(n0 + 1, -1), par1(n1 + 1, -1), par2(n, -1);
        vi V0(n0 + 1), V1(n1 + 1), V2(n);
        _for (u, n) {
            int i = ord[u];
            V2[i] = vs[u];
            if (col[u] != 1)
                V0[i] = vs[u];
            if (col[u] != 0)
                V1[max(i - n0, 0)] = vs[u];
        }
        _for (u, 1, n) {
            int a = ord[u], b = ord[par[u]];
            if (a > b)
                swap(a, b);
            par2[b] = a;
            if (col[u] != 1 && col[par[u]] != 1)
                par0[b] = a;
            if (col[u] != 0 && col[par[u]] != 0)
                par1[max(b - n0, 0)] = max(a - n0, 0);
        }
        f(par2, V2, n0, n1);
        dfs1(par0, V0, f);
        dfs1(par1, V1, f);
    }
    static void dfs2(vi& par, vi& vs, vcb& real, auto&& f) {
        const int n = len(vs);
        ASSERT(n > 1);
        if (n == 2) {
            if (real[0] && real[1])
                f(par, vs, vi{0, 1});
            return;
        }
        auto [sz, c] = find_centroid(par);
        vi col(n, -1);
        int take = 0;
        vi ord(n, -1);
        ord[c] = 0;
        int p = 1;
        _for (u, 1, n) {
            if (par[u] == c && take + sz[u] <= floor(n - 1, 2)) {
                ord[u] = p++;
                col[u] = 0;
                take += sz[u];
            }
        }
        _for (u, 1, n)
            if (!col[par[u]]) {
                ord[u] = p++;
                col[u] = 0;
            }
        int n0 = p - 1;
        for (int u = par[c]; u != -1; u = par[u]) {
            ord[u] = p++;
            col[u] = 1;
        }
        _for (u, n)
            if (u != c && ord[u] == -1) {
                ord[u] = p++;
                col[u] = 1;
            }
        ASSERT(p == n);
        int n1 = n - 1 - n0;
        vi par0(n0 + 1, -1), par1(n1 + 1, -1), par2(n, -1);
        vi V0(n0 + 1), V1(n1 + 1), V2(n);
        vcb real0(n0 + 1), real1(n1 + 1), real2(n);
        _for (u, n) {
            int i = ord[u];
            V2[i] = vs[u], real2[i] = real[u];
            if (col[u] != 1)
                V0[i] = vs[u], real0[i] = real[u];
            if (col[u] != 0)
                V1[max(i - n0, 0)] = vs[u], real1[max(i - n0, 0)] = real[u];
        }
        _for (u, 1, n) {
            int a = ord[u], b = ord[par[u]];
            if (a > b)
                swap(a, b);
            par2[b] = a;
            if (col[u] != 1 && col[par[u]] != 1)
                par0[b] = a;
            if (col[u] != 0 && col[par[u]] != 0)
                par1[max(b - n0, 0)] = max(a - n0, 0);
        }
        if (real[c]) {
            col[0] = 0;
            _for (i, 1, n)
                col[i] = real2[i] ? 1 : -1;
            f(par2, V2, col);
            real0[0] = real1[0] = real2[0] = 0;
        }
        col[0] = -1;
        _for (i, 1, n)
            col[i] = real2[i] ? (i <= n0 ? 0 : 1) : -1;
        f(par2, V2, col);
        dfs2(par0, V0, real0, f);
        dfs2(par1, V1, real1, f);
    }
    const G& g;
    vi par, new_par, V, vtoe;
    Centroid_Decomposition(const G& g): g(g), par(g.n, -1), new_par(g.n, -1), V(g.n) {
        const int n = g.n;
        if constexpr (VTOE)
            vtoe.resize(n);
        int l = 0, r = 0;
        V[r++] = 0;
        while (l < r) {
            int u = V[l++];
            foreach (v, g[u])
                if (v != par[u]) {
                    V[r++] = v;
                    par[v] = u;
                    if constexpr (VTOE)
                        vtoe[v] = v.id;
                }
        }
        vi new_idx(n);
        _for (i, n)
            new_idx[V[i]] = i;
        _for (i, 1, n)
            new_par[new_idx[i]] = new_idx[par[i]];
    }
    template <int MODE>
    void run(auto&& f) {
        if (g.n == 1)
            return;
        if constexpr (MODE == 0)
            dfs0(new_par, V, f);
        else if constexpr (MODE == 1)
            dfs1(new_par, V, f);
        else if constexpr (MODE == 2) {
            vcb real(g.n, 1);
            dfs2(new_par, V, real, f);
        }
        else
            static_assert(false);
    }
};
template <int MODE>
void centroid_decomposition(const auto& g, auto&& f) { Centroid_Decomposition<decltype(g), false>{g}.template run<MODE>(f); }