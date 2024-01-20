#pragma once
#include "graph/base.hpp"

template <typename T>
auto K_shortest_path(const DirectedGraph auto& g, int s, int t, int K) {
    const int n = g.n;

    struct Result {
        T dist;
        vi vs, es;
    };
    vc<Result> res;
    vc<std::pair<vi, vi>> nodes{{}};
    vc<std::tuple<T, vi, vi, int>> paths;

    vc<T> dist(n, inf<T>);
    vc<bool> ng_v(n);
    vc<bool> ng_e(g.m);
    vi par(n, -1);

    while (len(res) < K) {
        for (auto&& [es, ng_es]: nodes) {
            fill(par, -1);
            _for (i, n)
                ng_v[i] = false;
            _for (i, g.m)
                ng_e[i] = false;
            fill(dist, inf<T>);
            T pref_cost = 0;
            foreach (x, es)
                pref_cost += g.edges[x].cost;

            foreach (x, es)
                ng_v[g.edges[x].from] = 1, ng_e[x] = true;
            foreach (x, ng_es)
                ng_e[x] = true;
            // dijkstra
            PQG<std::pair<T, int>> que;
            auto add = [&](int v, T d, int u) {
                if (chkmin(dist[v], d)) {
                    que.emplace(d, v);
                    par[v] = u;
                }
            };
            int s0 = (es.empty() ? s : g.edges[es.back()].to);
            add(s0, pref_cost, -1);
            while (!que.empty()) {
                auto [d, u] = pop(que);
                if (d != dist[u])
                    continue;
                if (u == t)
                    break;
                foreach (v, g[u])
                    if (!ng_e[v.id] && !ng_v[v])
                        add(v, d + v.cost, v.id);
            }
            if (par[t] == -1)
                continue;
            vi add_e;
            {
                int u = t;
                while (u != s0) {
                    add_e.eb(par[u]);
                    u = g.edges[par[u]].from;
                }
            }
            reverse(add_e);
            int n = len(es);
            es.insert(es.end(), all(add_e));
            paths.eb(dist[t], es, ng_es, n);
        }
        if (paths.empty())
            break;
        std::pair best{-1, inf<T>};
        _for (i, len(paths))
            if (chkmin(best.second, std::get<0>(paths[i])))
                best.first = i;
        swap(paths[best.first], paths.back());
        auto [cost, es, ng_es, n] = pop(paths);
        vi vs{s};
        foreach (x, es)
            vs.eb(g.edges[x].to);
        res.eb(cost, std::move(vs), es);

        nodes.clear();
        _for (k, n, len(es)) {
            ng_es.eb(es[k]);
            nodes.eb(vi{es.begin(), es.begin() + k}, ng_es);
            ng_es.pop_back();
        }
    }
    return res;
}