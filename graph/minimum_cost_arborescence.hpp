#pragma once
#include "graph/base.hpp"
#include "ds/unionfind.hpp"
#include "utility/memory_pool.hpp"

namespace MinimumCostArborescenceImpl {
    template <typename G, int N>
    struct Solver {
        using T = G::cost_type;
        const G& g;

        Solver(const G& g): g(g) { ASSERT(g.n + g.m <= N); }

        vi calc(int root) {
            const int n = g.n, m = g.m;
            vc<np> q(n);
            foreach (e, g.edges)
                q[e.to] = meld(q[e.to], new_node(e.from, e.cost, e.id));
            vc<u8> used(n + m);
            used[root] = 2;
            vc<Edge> best(n + m);
            vi par(n + m, -1);
            vi rt(n + m);
            _for (i, n)
                rt[i] = i;
            UnionFind uf(n + m);
            int nxt = n;
            _for (s, n)
                if (!used[s]) {
                    vi path{s};
                    loop {
                        int a = path.back();
                        used[a] = 1;
                        if (!q[a])
                            return {};
                        best[a] = pop(q[a]);
                        int to = rt[uf[best[a].to]];
                        if (!used[to]) {
                            path.eb(to);
                            continue;
                        }
                        if (used[to] == 2)
                            break;
                        int u = nxt++;
                        q.eb();
                        loop {
                            int w = ::pop(path);
                            q[u] = meld(q[u], add(q[w], -best[w].cost));
                            uf.merge(u, w), par[w] = u;
                            used[w] = 2;
                            if (w == to)
                                break;
                        }
                        rt[uf[u]] = u;
                        path.eb(u);
                    }
                    foreach (u, path)
                        used[u] = 2;
                }
            vi r;
            vc<bool> done(nxt);
            done[root] = true;
            _for_r (u, nxt)
                if (!done[u]) {
                    int id = best[u].id;
                    r.eb(id);
                    int v = g.edges[id].to;
                    while (v != -1 && !done[v])
                        done[v] = true, v = par[v];
                }
            return r;
        }

    private:
        struct Edge {
            int to, id;
            T cost;
        };

        struct Node {
            Node *l, *r;
            Edge e;
            T lazy;
            int s;
        };
        Memory_Pool<Node, N> pool;
        using np = Node*;

        np new_node(int to, T cost, int id) { return pool.new_node({nullptr, nullptr, {to, id, cost}, 0, 1}); }
        static np add(np a, T x) {
            if (a)
                a->e.cost += x, a->lazy += x;
            return a;
        }
        np meld(np a, np b) {
            if (!a)
                return b;
            if (!b)
                return a;
            if (b->e.cost < a->e.cost)
                swap(a, b);
            b = add(b, -a->lazy);
            a->r = a->r ? meld(a->r, b) : b;
            if (!a->l || a->l->s < a->r->s)
                swap(a->l, a->r);
            a->s = (a->r ? a->r->s : 0) + 1;
            return a;
        }
        Edge pop(np& a) {
            Edge e = a->e;
            a = meld(add(a->l, a->lazy), add(a->r, a->lazy));
            return e;
        }
    };
} // namespace MinimumCostArborescenceImpl
template <typename T>
struct MinimumCostArborescenceResult {
    T cost;
    vi I;
};
template <int N = -1>
auto MinimumCostArborescence(const DirectedGraph auto& g, int root) {
    using G = std::decay_t<decltype(g)>;
    auto I = MinimumCostArborescenceImpl::Solver<G, N>{g}.calc(root);
    typename G::cost_type cost{};
    foreach (i, I)
        cost += g.edges[i].cost;
    return MinimumCostArborescenceResult{cost, std::move(I)};
};