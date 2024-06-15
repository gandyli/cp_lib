#pragma once
#include "ds/fastset.hpp"

template <int N>
struct Sortable_Array {
    static constexpr int LIM = N * 0.9;
    struct Node {
        Node *l, *r;
        int s;
    }* pool;
    using np = Node*;

    int id{};
    int n, m;
    FastSet s;
    vc<np> root;
    vcb rev;
    Sortable_Array(int m, const vi& a): pool(new Node[N]), n(len(a)), m(m) { init(a); }
    void set(int i, int x) {
        split_at(i), split_at(i + 1);
        rev[i] = 0;
        root[i] = new_node(0);
        set_rec(root[i], 0, m, x);
    }
    int get(int i) {
        auto dfs = [&](auto&& dfs, np u, int l, int r, int k) -> int {
            if (l + 1 == r)
                return l;
            int m = (l + r) >> 1;
            int s = u->l ? u->l->s : 0;
            if (k < s)
                return dfs(dfs, u->l, l, m, k);
            return dfs(dfs, u->r, m, r, k - s);
        };
        int j = s.prev(i);
        int k = i - j;
        if (rev[j])
            k = root[j]->s - k - 1;
        return dfs(dfs, root[j], 0, m, k);
    }
    vi get_all() {
        vi a;
        auto dfs = [&](auto&& dfs, np u, int l, int r, bool rev) {
            if (!u || !u->s)
                return;
            if (l + 1 == r) {
                a.insert(a.end(), u->s, l);
                return;
            }
            int m = (l + r) / 2;
            if (rev) {
                dfs(dfs, u->r, m, r, rev);
                dfs(dfs, u->l, l, m, rev);
            }
            else {
                dfs(dfs, u->l, l, m, rev);
                dfs(dfs, u->r, m, r, rev);
            }
        };
        _for (i, n)
            if (s.contains(i))
                dfs(dfs, root[i], 0, m, rev[i]);
        return a;
    }
    void sort(int l, int r, bool rev = false) {
        if (r - l <= 1)
            return;
        if (id > LIM)
            rebuild();
        split_at(l), split_at(r);
        loop {
            np u = root[l];
            int i = s.next(l + 1);
            if (i == r)
                break;
            root[l] = merge(u, root[i], 0, m);
            s.erase(i);
        }
        this->rev[l] = rev;
    }

private:
    np new_node(int s) {
        pool[id] = {nullptr, nullptr, s};
        return &pool[id++];
    }
    void rebuild() { init(get_all()); }
    void init(const vi& a) {
        id = 0;
        rev.assign(n, 0);
        root.assign(n, nullptr);
        s.build(n, [&](u32) { return 1; });
        _for (i, n)
            set_rec(root[i], 0, m, a[i]);
    }
    void split_at(int x) {
        if (x == n || s.contains(x))
            return;
        int a = s.prev(x), b = s.next(a + 1);
        s.insert(x);
        if (!rev[a]) {
            auto [u, v] = split(root[a], 0, m, x - a);
            root[a] = u, root[x] = v;
            rev[a] = rev[x] = 0;
        }
        else {
            auto [u, v] = split(root[a], 0, m, b - x);
            root[a] = v, root[x] = u;
            rev[a] = rev[x] = 1;
        }
    }
    std::pair<np, np> split(np u, int l, int r, int k) {
        if (k == 0)
            return {nullptr, u};
        if (k == u->s)
            return {u, nullptr};
        if (l + 1 == r) {
            int s = u->s - k;
            u->s = k;
            return {u, new_node(s)};
        }
        int m = (l + r) >> 1;
        int s = u->l ? u->l->s : 0;
        np v = new_node(0);
        if (k <= s) {
            auto [a, b] = split(u->l, l, m, k);
            v->l = b;
            v->r = u->r;
            u->l = a;
            u->r = nullptr;
        }
        else {
            auto [a, b] = split(u->r, m, r, k - s);
            u->r = a;
            v->l = nullptr;
            v->r = b;
        }
        update(u), update(v);
        return {u, v};
    }
    np merge(np u, np v, int l, int r) {
        if (!u)
            return v;
        if (!v)
            return u;
        if (l + 1 == r) {
            u->s += v->s;
            return u;
        }
        int m = (l + r) >> 1;
        u->l = merge(u->l, v->l, l, m);
        u->r = merge(u->r, v->r, m, r);
        update(u);
        return u;
    }
    void update(np u) { u->s = (u->l ? u->l->s : 0) + (u->r ? u->r->s : 0); }
    void set_rec(np& u, int l, int r, int k) {
        if (!u)
            u = new_node(0);
        if (l + 1 == r) {
            u->s = 1;
            return;
        }
        int m = (l + r) >> 1;
        if (k < m)
            set_rec(u->l, l, m, k);
        else
            set_rec(u->r, m, r, k);
        update(u);
    }
};