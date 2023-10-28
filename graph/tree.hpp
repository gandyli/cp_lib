#pragma once
#include "graph/base.hpp"

template <typename G>
class Tree {
private:
    void dfs1(int u) {
        auto& size = rid;
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
            wdep[v] = dep[u] + v.cost;
            fa[v] = u;
            vtoe[v] = v.id;
            dfs1(v);
            size[u] += size[v];
            if (size[v] > max) {
                max = size[v];
                swap(csr[l], csr[i]);
            }
        }
    }
    void dfs2(int u) {
        lid[u] = _id++;
        rid[u] += lid[u];
        id[lid[u]] = u;
        bool heavy = true;
        foreach (v, g[u])
            if (v != fa[u]) {
                top[v] = heavy ? top[u] : v;
                heavy = false;
                dfs2(v);
            }
    }

public:
    using T = typename G::cost_type;
    G& g;
    vi lid, rid, dep, top, fa, id, vtoe;
    Vec<T> wdep;
    int _id{};
    Tree(G& _g, int root = 0)
        : g(_g),
          lid(g.n),
          rid(g.n),
          dep(g.n),
          top(g.n, root),
          fa(g.n, root),
          id(g.n),
          vtoe(g.n),
          wdep(g.n) {
        build(root);
    }
    void build(int root) {
        dfs1(root);
        dfs2(root);
    }
    pi idx(int i) const { return {lid[i], rid[i]}; }
    bool in_subtree(int v, int u) const { return lid[v] <= lid[u] && lid[u] < rid[v]; }
    int size(int u) const { return rid[u] - lid[u]; }
    int size(int u, int r) const {
        if (u == r)
            return g.n;
        int v = jump(u, r, 1);
        if (in_subtree(u, v))
            return rid[u] - lid[u];
        return g.n - (rid[v] - lid[v]);
    }
    vi child(int u) {
        vi ret;
        foreach (v, g[u])
            if (v != fa[u])
                ret.eb(v);
        return ret;
    }
    int lca(int u, int v) const {
        while (top[u] != top[v]) {
            if (lid[u] < lid[v])
                swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
    int lca(int u, int v, int r) const {
        return lca(u, v) ^ lca(u, r) ^ lca(v, r);
    }
    int k_ancestor(int u, int k) const {
        while (k > dep[u] - dep[top[u]]) {
            k -= dep[u] - dep[fa[top[u]]];
            u = fa[top[u]];
        }
        return id[lid[u] - k];
    }
    int jump(int u, int v, int k) const {
        int w = lca(u, v);
        if (dep[u] - dep[w] >= k)
            return k_ancestor(u, k);
        k = (dep[u] + dep[v] - dep[w] * 2) - k;
        if (k < 0)
            return -1;
        return k_ancestor(v, k);
    }
    int dist(int u, int v) const { return dep[u] + dep[v] - dep[lca(u, v)] * 2; }
    T wdist(int u, int v) const { return wdep[u] + wdep[v] - wdep[lca(u, v)] * 2; }
    Vec<pi> path_decomposition(int u, int v, bool edge) const {
        Vec<pi> up, down;
        while (top[u] != top[v]) {
            if (lid[u] < lid[v]) {
                down.eb(lid[top[v]], lid[v]);
                v = fa[top[v]];
            }
            else {
                up.eb(lid[u], lid[top[u]]);
                u = fa[top[u]];
            }
        }
        if (lid[u] < lid[v])
            down.eb(lid[u] + edge, lid[v]);
        else if (lid[v] + edge <= lid[u])
            up.eb(lid[u], lid[v] + edge);
        reverse(down);
        up.insert(up.end(), all(down));
        return up;
    }
    vi path(int u, int v) const {
        vi ret;
        for (auto&& [a, b]: path_decomposition(u, v, false)) {
            if (a <= b) 
                _for (i, a, b + 1)
                    ret.eb(id[i]);
            else 
                _for_r (i, b, a + 1)
                    ret.eb(id[i]);
        }
        return ret;
    }
};