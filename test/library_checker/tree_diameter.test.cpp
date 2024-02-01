#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

#include "all.hpp"
#include "graph/utility.hpp"

int main() {
    dR(int, n);
    auto g = read_tree<int>(n, 0);
    auto [d, path] = diameter<i64>(g);
    print(d, len(path));
    print(path);
    return 0;
}