#pragma once
#include "../graph/base.hpp"

template <typename G>
class HeavyLightDecomposition {
private:
    void dfs1(int u) {
        size[u] = 1;
        int l = g.indptr[u], r = g.indptr[u + 1];
        auto& csr = g.csr_edges;
        _for_r (i, l, r - 1)
            if (!dep[csr[i + 1]])
                swap(csr[i], csr[i + 1]);
        int max = 0;
        _for (i, l, r) {
            auto& v = csr[i];
            if (v == fa[u])
                continue;
            dep[v] = dep[u] + 1;
            fa[v] = u;
            dfs1(v);
            size[u] += size[v];
            if (size[v] > max) {
                max = size[v];
                swap(csr[l], csr[i]);
            }
        }
    }

    void dfs2(int u) {
        dfn[u] = _id;
        id[_id++] = u;
        bool heavy = true;
        foreach (v, g[u])
            if (v != fa[u]) {
                top[v] = heavy ? top[u] : v;
                heavy = false;
                dfs2(v);
            }
    }

    // [u, v)
    [[nodiscard]] Vec<pi> ascend(int u, int v) const {
        Vec<pi> ret;
        while (top[u] != top[v]) {
            ret.eb(dfn[u], dfn[top[u]]);
            u = fa[top[u]];
        }
        if (u != v)
            ret.eb(dfn[u], dfn[v] + 1);
        return ret;
    }

    // (u, v]
    [[nodiscard]] Vec<pi> descend(int u, int v) const {
        if (u == v)
            return {};
        if (top[u] == top[v])
            return {{dfn[u] + 1, dfn[v]}};
        auto ret = descend(u, fa[top[v]]);
        ret.eb(dfn[top[v]], dfn[v]);
        return ret;
    }

public:
    G& g;
    vi size, dep, dfn, top, fa, id;
    int _id{};
    HeavyLightDecomposition(G& _g, int root = 0)
        : g(_g),
          size(g.n),
          dep(g.n),
          dfn(g.n),
          top(g.n, root),
          fa(g.n, root),
          id(g.n) {
        build(root);
    }

    void build(int root) {
        dfs1(root);
        dfs2(root);
    }

    [[nodiscard]] pi idx(int i) const { return {dfn[i], dfn[i] + size[i]}; }

    void path_query(int u, int v, bool vertex, auto&& f) {
        int l = lca(u, v);
        for (auto&& [a, b]: ascend(u, l)) {
            int s = a + 1, t = b;
            s > t ? f(t, s) : f(s, t);
        }
        if (vertex)
            f(dfn[l], dfn[l] + 1);
        for (auto&& [a, b]: descend(l, v)) {
            int s = a, t = b + 1;
            s > t ? f(t, s) : f(s, t);
        }
    }

    void path_noncommutative_query(int u, int v, bool vertex, auto& f) {
        int l = lca(u, v);
        for (auto&& [a, b]: ascend(u, l))
            f(a + 1, b);
        if (vertex)
            f(dfn[l], dfn[l] + 1);
        for (auto&& [a, b]: descend(l, v))
            f(a, b + 1);
    }

    void subtree_query(int u, bool vertex, auto&& f) {
        f(dfn[u] + !vertex, dfn[u]);
    }

    int lca(int x, int y) {
        while (top[x] != top[y]) {
            if (dfn[x] < dfn[y])
                swap(x, y);
            x = fa[top[x]];
        }
        return dep[x] < dep[y] ? x : y;
    }
    int k_ancestor(int x, int k) {
        while (k > dep[x] - dep[top[x]]) {
            k -= dep[x] - dep[fa[top[x]]];
            x = fa[top[x]];
        }
        return id[dfn[x] - k];
    }
    int jump(int x, int y, int k) {
        int z = lca(x, y);
        if (dep[x] - dep[z] >= k)
            return k_ancestor(x, k);
        k = (dep[x] + dep[y] - dep[z] * 2) - k;
        if (k < 0)
            return -1;
        return k_ancestor(y, k);
    }
    int dist(int a, int b) { return dep[a] + dep[b] - dep[lca(a, b)] * 2; }
};