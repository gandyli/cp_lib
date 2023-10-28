#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_C"

#include "all.hpp"
#include "string/longest_common_subsequence.hpp"

void test(auto&& a, auto&& b) {
    int lcs = longest_common_subsequence(a, b);
    auto r = longest_common_subsequence_result(a, b);
    assert(len(r) == lcs);
    for (auto&& [x, y]: r)
        assert(a[x] == b[y]);
    _for (i, lcs - 1) {
        assert(r[i].first < r[i + 1].first);
        assert(r[i].second < r[i + 1].second);
    }
}
int main() {
    multipleTests([&] {
        dR(str, s, t);
        test(s, t);
        print(longest_common_subsequence(s, t));
    });
    return 0;
}