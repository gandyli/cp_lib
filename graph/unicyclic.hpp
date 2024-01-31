#pragma once
#include "graph/tree.hpp"
#include "ds/unionfind.hpp"

template <UndirectedGraph G, bool weighted = false>
struct UnicyclicGraph {
    using T = G::cost_type;
    G& g;
    int n, root, out_eid, bottom;
    vi to, cycle;
    vc<u8> in_cycle;
    UnicyclicGraph(G& g): g(g), n(g.n), to(n, -1), in_cycle(n) {
        ASSERT(n == g.m);
        UnionFind uf(n);
        _for (eid, n) {
            auto&& e = g.edges[eid];
            if (!uf.merge(e.from, e.to)) {
                out_eid = eid;
                root = e.from;
                bottom = to[root] = e.to;
                break;
            }
        }
        vc<u8> vis(n);
        vi q{root};
        while (!q.empty()) {
            int u = pop(q);
            vis[u] = 1;
            foreach (v, g[u])
                if (!vis[v] && v.id != out_eid) {
                    to[v] = u;
                    q.eb(v);
                }
        }
        cycle = {bottom};
        in_cycle[bottom] = 1;
        while (cycle.back() != root)
            in_cycle[cycle.eb(to[cycle.back()])] = 1;
    }
    std::pair<Graph<T, true>, Tree<Graph<T, true>, weighted>> build(bool keep_eid = false) {
        static Graph<T, true> g(n);
        _for (eid, n)
            if (eid != out_eid) {
                auto&& e = this->g.edges[eid];
                int a = e.from, b = e.to;
                if (to[a] == b)
                    swap(a, b);
                g.add(a, b, e.cost, keep_eid ? eid : -1);
            }
        g.build();
        return {g, {g, root}};
    };
    auto dist(auto& tree, int u, int v, auto& D, auto out_cost) const {
        int u0 = tree.lca(u, bottom), v0 = tree.lca(v, bottom);
        auto d = std::abs(D[u0] - D[v0]);
        chkmin(d, D[bottom] + out_cost - d);
        return d + D[u] + D[v] - D[u0] - D[v0];
    }
    int dist(auto& tree, int u, int v) const { return dist(tree, u, v, tree.dep, 1); }
    T wdist(auto& tree, int u, int v) const requires weighted
    { return dist(tree, u, v, tree.wdep, g.edges[out_eid].cost); }
};