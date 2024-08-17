#pragma once
#include "utility/memory_pool.hpp"

template <typename Monoid, bool PERSISTENT, typename T = int, int N = -1>
struct Dynamic_SegTree_Sparse {
    using MX = Monoid;
    using X = MX::value_type;

    struct Node {
        T idx;
        Node *l, *r;
        X prod, x;
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    T L, R;
    Dynamic_SegTree_Sparse(T L, T R): L(L), R(R) {}
    np new_node() { return nullptr; }
    np new_node(T idx, const X& x) { return pool.new_node({idx, nullptr, nullptr, x, x}); }
    X prod(np u, T l, T r) {
        if (!u || l == r)
            return MX::unit();
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r, T ql, T qr) {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr || !u)
                return;
            if (ql == l && qr == r) {
                x = MX::op(x, u->prod);
                return;
            }
            T m = (l + r) >> 1;
            dfs(dfs, u->l, l, m, ql, qr);
            if (ql <= u->idx && u->idx < qr)
                x = MX::op(x, u->x);
            dfs(dfs, u->r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    X prod_all(np u) { return prod(u, L, R); }
    np set(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r, T i, X x) -> np {
            if (!u)
                return new_node(i, x);
            u = copy_node(u);
            if (u->idx == i) {
                u->x = x;
                update(u);
                return u;
            }
            T m = (l + r) >> 1;
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
    np multiply(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r, T i, X x) -> np {
            if (!u)
                return new_node(i, x);
            u = copy_node(u);
            if (u->idx == i) {
                u->x = MX::op(u->x, x);
                update(u);
                return u;
            }
            T m = (l + r) >> 1;
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
    np max_right(np u, auto&& check, T ql) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
            if (!u || r <= ql)
                return R;
            if (check(MX::op(x, u->prod))) {
                x = MX::op(x, u->prod);
                return R;
            }
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            if (ql <= u->idx) {
                x = MX::op(x, u->x);
                if (!check(x))
                    return u->idx;
            }
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    np min_left(np u, auto&& check, T qr) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
            if (!u || qr <= l)
                return L;
            if (check(MX::op(u->prod, x))) {
                x = MX::op(u->prod, x);
                return L;
            }
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->r, m, r);
            if (k != L)
                return k;
            if (u->idx < qr) {
                x = MX::op(u->x, x);
                if (!check(x))
                    return u->idx + 1;
            }
            return dfs(dfs, u->l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    void update(np u) {
        u->prod = u->x;
        if (u->l)
            u->prod = MX::op(u->l->prod, u->prod);
        if (u->r)
            u->prod = MX::op(u->prod, u->r->prod);
    }
    np copy_node(np u) {
        if (u && PERSISTENT)
            return pool.new_node(*u);
        return u;
    }
};