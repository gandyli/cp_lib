#define PROBLEM "https://atcoder.jp/contests/abc337/tasks/abc337_c"

#include "all.hpp"
#include "utility/cache.hpp"

int main() {
    dR(int, n);
    dRV(int, a, n);
    auto get = use_cache<int>([&](auto&& self, int u) -> int {
        if (a[u] == -1)
            return 0;
        return self(a[u] - 1) + 1;
    });
    vi ans(n);
    _for (i, n)
        ans[get(i)] = i + 1;
    print(ans);
    return 0;
}