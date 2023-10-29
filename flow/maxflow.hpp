#pragma once
#include "template.hpp"

template <typename T>
struct MaxFlow {
    struct Edge {
        int to, rev;
        T cap;
        T flow;
    };
    int n, s, t;
    Vec<Vec<Edge>> g;
    vi level;
    T ans{};

    MaxFlow(int n, int s, int t): n(n), s(s), t(t), g(n), level(n) {}
    void add(int u, int v, T cap) {
        if (u == v)
            return;
        g[u].eb(v, len(g[v]), cap, 0);
        g[v].eb(u, len(g[u]) - 1, 0, 0);
    }
    void build(T limit = inf<T>) {
        vi iter(n), q;
        auto bfs = [&] {
            fill(level, -1);
            level[s] = 0;
            q.clear();
            q.eb(s);
            _for (i, len(q)) {
                int u = q[i];
                foreach (e, g[u])
                    if (e.cap > 0 && level[e.to] == -1) {
                        level[e.to] = level[u] + 1;
                        if (e.to == t)
                            return true;
                        q.eb(e.to);
                    }
            }
            return false;
        };
        auto dfs = [&](auto&& dfs, int u, T limit) {
            if (u == s)
                return limit;
            T f{};
            for (int& i = iter[u]; i < len(g[u]); i++)
                if (auto&& e = g[u][i]; g[e.to][e.rev].cap > 0 && level[e.to] < level[u])
                    if (T d = dfs(dfs, e.to, min(limit - f, g[e.to][e.rev].cap)); d > 0) {
                        e.cap += d, e.flow -= d;
                        g[e.to][e.rev].cap -= d, g[e.to][e.rev].flow += d;
                        f += d;
                        if (f == limit)
                            return f;
                    }
            level[u] = n;
            return f;
        };
        while (ans < limit) {
            if (!bfs())
                break;
            fill(iter, 0);
            T f = dfs(dfs, t, limit - ans);
            if (!f)
                break;
            ans += f;
        }
    }
    T flow() { return ans; }
    Vec<bool> cut() {
        Vec<bool> r(n);
        _for (i, n)
            r[i] = level[i] != -1;
        return r;
    }
    vvi path() {
        vvi to(n);
        _for (i, n)
            foreach (e, g[i])
                to[i].insert(to[i].end(), e.flow, e.to);
        Vec<bool> vis(n);
        vvi r(ans);
        _for (i, ans) {
            auto&& path = r[i];
            path = {s};
            vis[s] = true;
            while (path.back() != t) {
                int u = pop(to[path.back()]);
                while (vis[u])
                    vis[pop(path)] = false;
                path.eb(u);
                vis[u] = true;
            }
            foreach (u, path)
                vis[u] = false;
        }
        return r;
    }
};