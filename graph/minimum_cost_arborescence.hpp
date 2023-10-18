#pragma once
#include "base.hpp"
#include "../ds/dsu.hpp"

namespace MinimumCostArborescenceImpl {
    template <typename G, int N>
    struct Solver {
        using T = typename G::cost_type;
        const G& g;

        Solver(const G& g): g(g) { assert(g.n + g.m <= N); }

        vi calc(int root) {
            int n = g.n, m = g.m;
            Vec<Node*> q(n);
            foreach (e, g.edges)
                q[e.to] = meld(q[e.to], new_node(e.from, e.cost, e.id));
            Vec<u8> used(n + m);
            used[root] = 2;
            Vec<Edge> best(n + m);
            vi par(n + m, -1);
            vi rt(n + m);
            _for (i, n)
                rt[i] = i;
            DSU dsu(n + m);
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
                        int to = rt[dsu.leader(best[a].to)];
                        if (!used[to]) {
                            path.eb(to);
                            continue;
                        }
                        if (used[to] == 2)
                            break;
                        int u = nxt++;
                        q.eb();
                        loop {
                            int w = path.back();
                            path.pop_back();
                            q[u] = meld(q[u], add(q[w], -best[w].cost));
                            dsu.merge(u, w), par[w] = u;
                            used[w] = 2;
                            if (w == to)
                                break;
                        }
                        rt[dsu.leader(u)] = u;
                        path.eb(u);
                    }
                    foreach (u, path)
                        used[u] = 2;
                }
            vi ret;
            Vec<bool> done(nxt);
            done[root] = true;
            _for_r (u, nxt)
                if (!done[u]) {
                    int id = best[u].id;
                    ret.eb(id);
                    int v = g.edges[id].to;
                    while (v != -1 && !done[v])
                        done[v] = true, v = par[v];
                }
            return ret;
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
        }* pool{new Node[N]};
        int pid{};

        Node* new_node(int to, T cost, int id) {
            pool[pid] = {nullptr, nullptr, {to, id, cost}, 0, 1};
            return &pool[pid++];
        }
        static Node* add(Node* a, T x) {
            if (a)
                a->e.cost += x, a->lazy += x;
            return a;
        }
        Node* meld(Node* a, Node* b) {
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
        Edge pop(Node*& a) {
            Edge e = a->e;
            a = meld(add(a->l, a->lazy), add(a->r, a->lazy));
            return e;
        }
    };
} // namespace MinimumCostArborescenceImpl
template <int N>
auto MinimumCostArborescence(auto& g, int root) {
    using G = std::decay_t<decltype(g)>;
    auto I = MinimumCostArborescenceImpl::Solver<G, N>{g}.calc(root);
    typename G::cost_type cost{};
    foreach (i, I)
        cost += g.edges[i].cost;
    return std::pair{cost, std::move(I)};
};