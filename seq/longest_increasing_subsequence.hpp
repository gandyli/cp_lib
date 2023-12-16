#pragma once
#include "template.hpp"

template <typename T, bool strong = true>
std::pair<int, vi> longest_increasing_sequence(const Vec<T>& a) {
    const int n = len(a);
    Vec dp(n, inf<T>);
    vi lis_rank(n);
    _for (i, n) {
        int j = (strong ? LB(dp, a[i]) : UB(dp, a[i]));
        dp[j] = a[i];
        lis_rank[i] = j + 1;
    }
    return {max(lis_rank), std::move(lis_rank)};
}
template <typename T, bool strong = true>
vi longest_increasing_sequence_id(const Vec<T>& a) {
    const int n = len(a);
    auto [m, d] = longest_increasing_sequence<T, strong>(a);
    vi I;
    I.reserve(m);
    T x = inf<T>;
    _for_r (i, n)
        if (d[i] == m && (strong ? a[i] < x : a[i] <= x)) {
            I.eb(i);
            m--;
            x = a[i];
        }
    reverse(I);
    return I;
}