#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A"

#include "all.hpp"
#include "graph/minimum_hamiltonian_cycle.hpp"

int main() {
    dR(int, n, m);
    auto g = read_graph<int, true>(n, m, 0);
    auto [cost, _] = minimum_hamiltonian_cycle<int>(g);
    if (cost == inf<int>)
        print(-1);
    else
        print(cost);
    return 0;
}