#define PROBLEM "https://judge.yosupo.jp/problem/persistent_unionfind"

#include "all.hpp"
#include "ds/dynamic_unionfind.hpp"

int main() {
    dR(int, n, q);
    Dynamic_UnionFind<true> uf;
    using Node = decltype(uf)::Node;
    vc<Node*> roots{uf.new_node()};
    _for (q) {
        dR(int, t, k, u, v), k++;
        Node* r = roots[k];
        if (t == 0) {
            auto [nr, b] = uf.merge(r, u, v);
            r = nr;
        }
        else
            print(uf.same(r, u, v));
        roots.eb(r);
    }
    return 0;
}