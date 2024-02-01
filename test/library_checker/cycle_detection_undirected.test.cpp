#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection_undirected"

#include "all.hpp"
#include "graph/find_cycle.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph(n, m, 0);
    auto [vs, es] = find_cycle(g);
    if (vs.empty()) {
        print(-1);
    }
    else {
        print(len(vs));
        print(vs);
        print(es);
    }
    return 0;
}