#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection"

#include "all.hpp"
#include "graph/find_cycle.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<void, true>(n, m, 0);
    auto [vs, es] = find_cycle(g);
    if (vs.empty()) {
        print(-1);
    }
    else {
        print(len(es));
        io.displayArray(es, '\n');
    }
    return 0;
}