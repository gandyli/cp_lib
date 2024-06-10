#pragma once
#include "utility/memory_pool.hpp"

template <int Σ, typename F = decltype([](int x) { return x; }), int N = -1>
struct Trie {
    Memory_Pool<std::array<int, Σ>, N> child;
    vi suffix_link, bfs, words;
    const F f;
    Trie(): f{} { child.new_node(), child.new_node(); }
    Trie(const F& f): f(f) { child.new_node(), child.new_node(); }
    int insert(auto&& s) {
        int u = 1;
        foreach (c, s) {
            int x = f(c);
            if (!child[u][x])
                child[u][x] = child.new_node_id();
            u = child[u][x];
        }
        words.eb(u);
        return u;
    }
    int insert_single(int u, int c) {
        int x = f(c);
        if (child[u][x])
            return child[u][x];
        return child[u][x] = child.new_node_id();
    }
    int query(auto&& s) {
        int u = 1;
        foreach (c, s) {
            int x = f(c);
            if (!child[u][x])
                return 0;
            u = child[u][x];
        }
        return u;
    }
    void calc_suffix_link(bool upd_child) {
        suffix_link.assign(len(child), -1);
        bfs.resize(len(child));
        int p = 0, q = 0;
        bfs[q++] = 1;
        while (p < q) {
            int u = bfs[p++];
            _for (x, Σ) {
                int v = child[u][x];
                if (!v)
                    continue;
                bfs[q++] = v;
                int f = suffix_link[u];
                while (f != -1 && !child[f][x])
                    f = suffix_link[f];
                suffix_link[v] = f == -1 ? 1 : child[f][x];
            }
        }
        if (upd_child)
            foreach (u, bfs) {
                _for (x, Σ) {
                    if (!child[u][x]) {
                        int f = suffix_link[u];
                        child[u][x] = f == -1 ? 1 : child[f][x];
                    }
                }
            }
    }
    vi calc_count() {
        ASSERT(!suffix_link.empty());
        vi cnt(len(child));
        foreach (u, words)
            cnt[u]++;
        foreach (u, bfs)
            if (u > 1)
                cnt[u] += cnt[suffix_link[u]];
        return cnt;
    }
};