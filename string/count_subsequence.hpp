#pragma once
#include "ds/hashmap.hpp"

template <typename mint>
mint count_subsequence(auto&& s) {
    HashMap<int, mint> dp;
    mint ans = 1;
    foreach (x, s) {
        mint t = dp[x];
        dp[x] = ans;
        ans += ans - t;
    }
    return ans - 1;
}