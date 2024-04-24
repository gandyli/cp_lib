#define PROBLEM "https://atcoder.jp/contests/past202010-open/tasks/past202010_f"

#include "all.hpp"
#include "ds/counter.hpp"

int main() {
    dR(int, n, k), k--;
    dRV(str, a, n);
    auto C = Counter(a).most_common();
    vc<str> K(len(C));
    vi V(len(C));
    _for (i, len(C)) {
        K[i] = C[i].first;
        V[i] = C[i].second;
    }
    if (Counter(V)[V[k]] == 1) {
        print(K[k]);
    }
    else {
        print("AMBIGUOUS");
    }
    return 0;
}