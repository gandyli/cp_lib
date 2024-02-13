#pragma once
#include "utility/memory_pool.hpp"

template <typename Monoid, bool PERSISTENT, int N = -1>
struct Dynamic_SegTree_Sparse {
    using M = Monoid;
    using X = M::value_type;

    struct Node {
        i64 idx;
        Node *l, *r;
        X prod, x;
    };
    Memory_Pool<Node, N> pool;

    i64 L, R;
    Dynamic_SegTree_Sparse(i64 L, i64 R): L(L), R(R) {}
    Node* new_node() { return nullptr; }
    Node* new_node(i64 idx, const X& x) { return pool.new_node({idx, nullptr, nullptr, x, x}); }
    X prod(Node* u, i64 l, i64 r) {
        if (!u || l == r)
            return M::unit();
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r, i64 ql, i64 qr) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr || !u)
                return;
            if (ql == l && qr == r) {
                x = M::op(x, u->prod);
                return;
            }
            i64 m = (l + r) >> 1;
            dfs(dfs, u->l, l, m, ql, qr);
            if (ql <= u->idx && u->idx < qr)
                x = M::op(x, u->x);
            dfs(dfs, u->r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    X prod_all(Node* u) { return prod(u, L, R); }
    Node* set(Node* u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r, i64 i, X x) -> Node* {
            if (!u)
                return new_node(i, x);
            u = copy_node(u);
            if (u->idx == i) {
                u->x = x;
                update(u);
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m) {
                if (u->idx < i)
                    swap(u->idx, i), swap(u->x, x);
                u->l = dfs(dfs, u->l, l, m, i, x);
            }
            else {
                if (i < u->idx)
                    swap(u->idx, i), swap(u->x, x);
                u->r = dfs(dfs, u->r, m, r, i, x);
            }
            update(u);
            return u;
        };
        return dfs(dfs, u, L, R, i, x);
    }
    Node* multiply(Node* u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r, i64 i, X x) -> Node* {
            if (!u)
                return new_node(i, x);
            u = copy_node(u);
            if (u->idx == i) {
                u->x = M::op(u->x, x);
                update(u);
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m) {
                if (u->idx < i)
                    swap(u->idx, i), swap(u->x, x);
                u->l = dfs(dfs, u->l, l, m, i, x);
            }
            else {
                if (i < u->idx)
                    swap(u->idx, i), swap(u->x, x);
                u->r = dfs(dfs, u->r, m, r, i, x);
            }
            update(u);
            return u;
        };
        return dfs(dfs, u, L, R, i, x);
    }
    Node* max_right(Node* u, auto&& check, i64 ql) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> i64 {
            if (!u || r <= ql)
                return R;
            if (check(M::op(x, u->prod))) {
                x = M::op(x, u->prod);
                return R;
            }
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            if (ql <= u->idx) {
                x = M::op(x, u->x);
                if (!check(x))
                    return u->idx;
            }
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    Node* min_left(Node* u, auto&& check, i64 qr) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> i64 {
            if (!u || qr <= l)
                return L;
            if (check(M::op(u->prod, x))) {
                x = M::op(u->prod, x);
                return L;
            }
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, u->r, m, r);
            if (k != L)
                return k;
            if (u->idx < qr) {
                x = M::op(u->x, x);
                if (!check(x))
                    return u->idx + 1;
            }
            return dfs(dfs, u->l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    void update(Node* u) {
        u->prod = u->x;
        if (u->l)
            u->prod = M::op(u->l->prod, u->prod);
        if (u->r)
            u->prod = M::op(u->prod, u->r->prod);
    }
    Node* copy_node(Node* u) {
        if (u && PERSISTENT)
            return pool.new_node(*u);
        return u;
    }
};