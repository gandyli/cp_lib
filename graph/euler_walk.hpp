#pragma once
#include "graph/vs_to_es.hpp"

struct EulerWalkResult {
    vi vs, es;
};
EulerWalkResult euler_walk(const auto& g, int s = -1) {
    const int n = g.n, m = g.m;

    if (s == -1) {
        vi deg(n);
        foreach (e, g.edges)
            if constexpr (g.is_directed())
                deg[e.from]++, deg[e.to]--;
            else
                deg[e.from]++, deg[e.to]++;
        if constexpr (g.is_directed()) {
            s = max_element(deg) - deg.begin();
            if (!deg[s])
                s = m ? g.edges[0].from : 0;
        }
        else
            s = BLK {
                _for (i, n)
                    if (deg[i] & 1)
                        return i;
                return m ? g.edges[0].from : 0;
            };
    }
    if (!m)
        return {{s}, {}};
    vi D(n), is(all(g.indptr) - 1), st{s}, vs;
    D[s]++;
    vc<bool> vis(m);
    while (!st.empty()) {
        int u = st.back(), &i = is[u];
        if (i == g.indptr[u + 1]) {
            vs.eb(u);
            pop(st);
            continue;
        }
        auto v = g.csr_edges[i++];
        if (!vis[v.id]) {
            vis[v.id] = true;
            D[u]--, D[v]++;
            st.eb(v);
        }
    }
    if (len(vs) != m + 1)
        return {};
    foreach (x, D)
        if (x < 0)
            return {};
    reverse(vs);
    auto es = vs_to_es(g, vs);
    return {std::move(vs), std::move(es)};
}