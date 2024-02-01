#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

#include "all.hpp"
#include "graph/tree.hpp"

int main() {
    dR(int, n, q);
    auto g = read_tree(n, 0);
    Tree tree(g);
    _for (q) {
        dR(int, u, v, x);
        print(tree.jump(u, v, x));
    }
    return 0;
}