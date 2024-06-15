#pragma once
#include "graph/base.hpp"

struct FindCycleResult {
    vi vs, es;
};
FindCycleResult find_cycle(const DirectedGraph auto& g) {
    const int n = g.n;
    vc<u8> vis(n);
    vc<pi> par(n);
    vi es;
    auto dfs = [&](auto&& dfs, int u) -> void {
        vis[u] = 1;
        foreach (v, g[u]) {
            if (!es.empty())
                return;
            if (!vis[v]) {
                par[v] = {u, v.id};
                dfs(dfs, v);
            }
            else if (vis[v] == 1) {
                es.eb(v.id);
                while (u != v) {
                    es.eb(par[u].second);
                    u = par[u].first;
                }
                return;
            }
        }
        vis[u] = 2;
    };
    _for (u, n)
        if (!vis[u])
            dfs(dfs, u);
    if (es.empty())
        return {};
    vi nxt(n, -1);
    foreach (e, es)
        nxt[g.edges[e].from] = e;
    foreach (e, g.edges) {
        int u = e.from, v = e.to;
        if (nxt[u] != -1 && nxt[v] != -1 && g.edges[nxt[u]].to != v) {
            while (u != v) {
                int t = g.edges[nxt[u]].to;
                nxt[u] = -1;
                u = t;
            }
            nxt[e.from] = e.id;
        }
    }
    es.clear();
    vi vs;
    _for (u, n)
        if (nxt[u] != -1) {
            int t = u;
            do {
                vs.eb(t);
                es.eb(nxt[t]);
                t = g.edges[nxt[t]].to;
            } while (t != u);
            break;
        }
    return {std::move(vs), std::move(es)};
}

FindCycleResult find_cycle(const UndirectedGraph auto& g, bool minimal = false) {
    const int n = g.n, m = g.m;
    vi dep(n, -1);
    vcb vis(m);
    vi par(n, -1);
    auto dfs = [&](auto&& dfs, int u, int d) {
        dep[u] = d;
        foreach (v, g[u])
            if (!vis[v.id]) {
                if (dep[v] != -1)
                    return minimal ? u : v.id;
                vis[v.id] = 1;
                par[v] = v.id;
                int t = dfs(dfs, v, d + 1);
                if (t != -1)
                    return t;
            }
        return -1;
    };
    vi vs, es;
    _for (u, n)
        if (dep[u] == -1) {
            int w = dfs(dfs, u, 0);
            if (w == -1)
                continue;
            int a;
            if (minimal) {
                int b = -1, back_e = -1;
                loop {
                    foreach (v, g[w]) {
                        if (vis[v.id])
                            continue;
                        if (dep[v] > dep[w] || dep[v] == -1)
                            continue;
                        b = w, back_e = v.id;
                    }
                    if (w == u)
                        break;
                    auto&& e = g.edges[par[w]];
                    w = e.from + e.to - w;
                }
                es.eb(back_e);
                a = g.edges[back_e].from + g.edges[back_e].to - b;
            }
            else {
                es.eb(w);
                a = dep[g.edges[w].from] > dep[g.edges[w].to] ? g.edges[w].to : g.edges[w].from;
            }
            vs.eb(a);
            loop {
                int x = vs.back();
                auto&& e = g.edges[es.back()];
                int y = e.from + e.to - x;
                if (y == a)
                    break;
                vs.eb(y);
                es.eb(par[y]);
            }
            break;
        }
    return {std::move(vs), std::move(es)};
}