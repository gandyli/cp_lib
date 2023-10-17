#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include "all.hpp"
#include "graph/dijkstra.hpp"
#include "graph/restore_path.hpp"

int main() {
    dR(int, n, m, s, t);
    Graph<int, true> g(n);

    g.read_graph(m, true, 0);
    auto [dist, prev] = dijkstra<i64>(g, s);
    if (dist[t] == inf<i64>)
        writeln(-1);
    else {
        auto path = restore_path(prev, t);
        writeln(dist[t], ' ', len(path) - 1);
        _for (i, len(path) - 1)
            writeln(path[i], ' ', path[i + 1]);
    }
    return 0;
}