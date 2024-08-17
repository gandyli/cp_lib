#define PROBLEM "https://atcoder.jp/contests/abc362/tasks/abc362_f"

#include "all.hpp"
#include "graph/find_centroids.hpp"

int main() {
    dR(int, n);
    auto g = read_tree(n);
    auto [c, _] = find_centroids(g);
    vi V;
    auto dfs = [&](auto&& dfs, int u, int f) -> void {
        V.eb(u);
        foreach (v, g[u])
            if (v != f)
                dfs(dfs, v, u);
    };
    foreach (u, g[c])
        dfs(dfs, u, c);
    V.eb(c);
    _for (i, n / 2)
        print(V[i] + 1, V[i + n / 2] + 1);
    return 0;
}