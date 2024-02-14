#pragma once
#include "ds/meldable_heap.hpp"
#include "graph/dijkstra.hpp"

template <typename T, int N = -1>
vc<T> K_shortest_walk(const DirectedGraph auto& g, int s, int t, int k) {
    const int n = g.n;
    auto rg = g.reverse();
    auto [dis, par] = dijkstra<T>(rg, t);
    if (dis[s] == inf<T>)
        return vc(k, inf<T>);
    using Heap = Meldable_Heap<std::pair<T, int>, true, N>;
    using np = Heap::np;
    Heap heap;

    vc<np> nodes(n);
    vc<bool> vis(n);
    vi st{t};
    vis[t] = true;
    while (!st.empty()) {
        int u = pop(st);
        bool done = false;
        foreach (v, g[u])
            if (dis[v] != inf<T>) {
                if (!done && par[u] == v && dis[u] == dis[v] + v.cost) {
                    done = true;
                    continue;
                }
                T cost = -dis[u] + v.cost + dis[v];
                nodes[u] = heap.push(nodes[u], {cost, v});
            }
        foreach (v, rg[u])
            if (!vis[v] && par[v] == u) {
                nodes[v] = heap.meld(nodes[v], nodes[u]);
                vis[v] = true;
                st.eb(v);
            }
    }
    T base = dis[s];
    vc<T> r{base};
    r.reserve(k);
    if (nodes[s]) {
        auto cmp = [&](auto&& a, auto&& b) { return a.first > b.first; };
        std::priority_queue<std::pair<T, np>, vc<std::pair<T, np>>, decltype(cmp)> q(cmp);
        q.emplace(base + heap.top(nodes[s]).first, nodes[s]);
        while (!q.empty()) {
            auto [d, n] = pop(q);
            if (r.eb(d); len(r) == k)
                break;
            if (n->l)
                q.emplace(d + (n->l->x.first) - (n->x.first), n->l);
            if (n->r)
                q.emplace(d + (n->r->x.first) - (n->x.first), n->r);
            np m = nodes[n->x.second];
            if (m)
                q.emplace(d + m->x.first, m);
        }
    }
    while (len(r) < k)
        r.eb(inf<T>);
    return r;
}