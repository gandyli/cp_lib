#define PROBLEM "https://atcoder.jp/contests/abc268/tasks/abc268_Ex"

#include "all.hpp"
#include "string/trie.hpp"

int main() {
    dR(str, s);
    dR(int, n);
    Trie<26, decltype([](char c) { return c - 'a'; })> trie;
    _for (n) {
        dR(str, s);
        trie.insert(s);
    }
    trie.calc_suffix_link(true);
    auto cnt = trie.calc_count();
    int ans = 0, u = 1;
    foreach (c, s) {
        int x = c - 'a';
        u = trie.child[u][x];
        if (cnt[u]) {
            ans++;
            u = 1;
        }
    }
    print(ans);
    return 0;
}