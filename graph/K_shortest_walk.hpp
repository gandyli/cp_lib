#pragma once
#include "../ds/meldable_heap.hpp"
#include "dijkstra.hpp"

template <typename T, int N>
Vec<T> K_shortest_walk(const DirectedGraph auto& g, int s, int t, int k) {
    int n = g.n;
    auto rg = g.reverse();
    auto [dis, par] = dijkstra<T>(rg, t);
    if (dis[s] == inf<T>)
        return Vec(k, inf<T>);
    using Heap = Meldable_Heap<std::pair<T, int>, true, N>;
    using Node = Heap::Node;
    Heap heap;

    Vec<Node*> nodes(n);
    Vec<bool> vis(n);
    vi st{t};
    vis[t] = true;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
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
    Vec<T> ret{base};
    ret.reserve(k);
    if (nodes[s]) {
        auto cmp = [&](auto&& a, auto&& b) { return a.first > b.first; };
        std::priority_queue<std::pair<T, Node*>, Vec<std::pair<T, Node*>>, decltype(cmp)> q(cmp);
        q.emplace(base + heap.top(nodes[s]).first, nodes[s]);
        while (!q.empty()) {
            auto [d, n] = q.top();
            q.pop();
            if (ret.eb(d); len(ret) == k)
                break;
            if (n->l)
                q.emplace(d + (n->l->x.first) - (n->x.first), n->l);
            if (n->r)
                q.emplace(d + (n->r->x.first) - (n->x.first), n->r);
            Node* m = nodes[n->x.second];
            if (m)
                q.emplace(d + m->x.first, m);
        }
    }
    while (len(ret) < k)
        ret.eb(inf<T>);
    return ret;
}