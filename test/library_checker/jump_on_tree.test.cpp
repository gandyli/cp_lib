#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

#include "all.hpp"
#include "tree/heavy_light_decomposition.hpp"

int main() {
    dR(int, n, q);
    Graph<bool, false> g(n);
    g.read_tree(false, 0);
    HeavyLightDecomposition hld(g);
    _for (q) {
        dR(int, u, v, x);
        writeln(hld.jump(u, v, x));
    }
    return 0;
}