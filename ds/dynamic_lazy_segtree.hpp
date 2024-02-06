#pragma once
#include "ds/unit_prod.hpp"
#include "utility/memory_pool.hpp"

template <typename ActedMonoid, bool PERSISTENT, int N = -1, typename F = Unit_Prod<typename ActedMonoid::Monoid_X>>
struct Dynamic_Lazy_SegTree {
    using AM = ActedMonoid;
    using MX = ActedMonoid::Monoid_X;
    using MA = ActedMonoid::Monoid_A;
    using X = MX::value_type;
    using A = MA::value_type;

    struct Node {
        int l, r;
        X x;
        A lazy;
    };
    Memory_Pool<Node, N> pool;

    i64 L, R;
    F default_prod;
    Dynamic_Lazy_SegTree(i64 L, i64 R, F default_prod = {}): L(L), R(R), default_prod(default_prod) { pool.new_node(); }
    int new_node(const X& x) { return pool.new_node_id({0, 0, x, MA::unit()}); }
    int new_node(i64 l, i64 r) { return new_node(default_prod(l, r)); }
    int new_node() { return new_node(L, R); }
    int new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        auto dfs = [&](auto&& dfs, i64 l, i64 r) -> int {
            if (l == r)
                return 0;
            if (l + 1 == r)
                return new_node(a[l]);
            i64 m = (l + r) >> 1;
            int lr = dfs(dfs, l, m), rr = dfs(dfs, m, r);
            X x = MX::op(pool[lr].x, pool[rr].x);
            int u = new_node(x);
            pool[u].l = lr, pool[u].r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    int set(int u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> int {
            if (l + 1 == r) {
                u = copy_node(u);
                pool[u].x = x;
                pool[u].lazy = MA::unit();
                return u;
            }
            pushdown(u, l, r);
            i64 m = (l + r) >> 1;
            if (!pool[u].l)
                pool[u].l = new_node(l, m);
            if (!pool[u].r)
                pool[u].r = new_node(m, r);
            u = copy_node(u);
            if (i < m)
                pool[u].l = dfs(dfs, pool[u].l, l, m);
            else
                pool[u].r = dfs(dfs, pool[u].r, m, r);
            pool[u].x = MX::op(pool[pool[u].l].x, pool[pool[u].r].x);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    int multiply(int u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> int {
            if (l + 1 == r) {
                u = copy_node(u);
                pool[u].x = MX::op(pool[u].x, x);
                pool[u].lazy = MA::unit();
                return u;
            }
            pushdown(u, l, r);
            i64 m = (l + r) >> 1;
            if (!pool[u].l)
                pool[u].l = new_node(l, m);
            if (!pool[u].r)
                pool[u].r = new_node(m, r);
            u = copy_node(u);
            if (i < m)
                pool[u].l = dfs(dfs, pool[u].l, l, m);
            else
                pool[u].r = dfs(dfs, pool[u].r, m, r);
            pool[u].x = MX::op(pool[pool[u].l].x, pool[pool[u].r].x);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    X prod(int u, i64 l, i64 r) {
        if (!u || l == r)
            return MX::unit();
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r, i64 ql, i64 qr, A lazy) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return;
            if (!u) {
                x = MX::op(x, AM::act(default_prod(ql, qr), lazy, qr - ql));
                return;
            }
            if (ql == l && qr == r) {
                x = MX::op(x, AM::act(pool[u].x, lazy, r - l));
                return;
            }
            i64 m = (l + r) >> 1;
            lazy = MA::op(pool[u].lazy, lazy);
            dfs(dfs, pool[u].l, l, m, ql, qr, lazy);
            dfs(dfs, pool[u].r, m, r, ql, qr, lazy);
        };
        dfs(dfs, u, L, R, l, r, MA::unit());
        return x;
    }
    X prod_all(int u) { return prod(u, L, R); }
    int apply(int u, i64 l, i64 r, const A& a) {
        if (l == r)
            return u;
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r, i64 ql, i64 qr) -> int {
            if (!u)
                u = new_node(l, r);
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return u;
            if (ql == l && qr == r) {
                u = copy_node(u);
                pool[u].x = AM::act(pool[u].x, a, r - l);
                pool[u].lazy = MA::op(pool[u].lazy, a);
                return u;
            }
            pushdown(u, l, r);
            i64 m = (l + r) >> 1;
            u = copy_node(u);
            pool[u].l = dfs(dfs, pool[u].l, l, m, ql, qr);
            pool[u].r = dfs(dfs, pool[u].r, m, r, ql, qr);
            pool[u].x = MX::op(pool[pool[u].l].x, pool[pool[u].r].x);
            return u;
        };
        return dfs(dfs, u, L, R, l, r);
    }
    i64 max_right(int u, auto&& check, i64 ql) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (r <= ql)
                return R;
            if (ql <= l && check(MX::op(x, pool[u].x))) {
                x = MX::op(x, pool[u].x);
                return R;
            }
            if (l + 1 == r)
                return l;
            pushdown(u, l, r);
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, pool[u].l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, pool[u].r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    i64 min_left(int u, auto&& check, i64 qr) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (qr <= l)
                return L;
            if (r <= qr && check(MX::op(pool[u].x, x))) {
                x = MX::op(pool[u].x, x);
                return L;
            }
            if (l + 1 == r)
                return r;
            pushdown(u, l, r);
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, pool[u].r, m, r);
            if (k != L)
                return k;
            return dfs(dfs, pool[u].l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    int copy_node(int u) {
        if (u && PERSISTENT)
            return pool.new_node_id(pool[u]);
        return u;
    }
    void pushdown(int u, i64 l, i64 r) {
        if (pool[u].lazy == MA::unit())
            return;
        i64 m = (l + r) >> 1;
        pool[u].l = pool[u].l ? copy_node(pool[u].l) : new_node(l, m);
        pool[pool[u].l].x = AM::act(pool[pool[u].l].x, pool[u].lazy, m - l);
        pool[pool[u].l].lazy = MA::op(pool[pool[u].l].lazy, pool[u].lazy);
        pool[u].r = pool[u].r ? copy_node(pool[u].r) : new_node(m, r);
        pool[pool[u].r].x = AM::act(pool[pool[u].r].x, pool[u].lazy, r - m);
        pool[pool[u].r].lazy = MA::op(pool[pool[u].r].lazy, pool[u].lazy);
        pool[u].lazy = MA::unit();
    }
};