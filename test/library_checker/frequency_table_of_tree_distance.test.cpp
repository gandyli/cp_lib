#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#include "all.hpp"
#include "poly/ntt_avx2.hpp"
#include "graph/tree_all_distances.hpp"

int main() {
    dR(int, n);
    auto g = read_tree(n, 0);
    auto ans = tree_all_distances(g);
    ans.erase(ans.begin());
    foreach (x, ans)
        x >>= 1;
    print(ans);
    return 0;
}