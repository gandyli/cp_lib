#define PROBLEM "https://judge.yosupo.jp/problem/persistent_queue"

#include "all.hpp"
#include "ds/dynamic_array.hpp"

int main() {
    dR(int, q);
    Dynamic_Array<int, true> a(q);
    using np = decltype(a)::np;
    vc<np> roots{a.new_node()};
    vi L{0}, R{0};
    _for (q) {
        dR(int, op, t), t++;
        np u = roots[t];
        int l = L[t], r = R[t];
        if (op == 0) {
            dR(int, x);
            u = a.set(u, r++, x);
        }
        else {
            print(a.get(u, l++));
        }
        roots.eb(u);
        L.eb(l);
        R.eb(r);
    }
    return 0;
}