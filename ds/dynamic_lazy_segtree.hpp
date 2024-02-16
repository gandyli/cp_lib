#pragma once
#include "ds/unit_prod.hpp"
#include "utility/memory_pool.hpp"

template <typename ActedMonoid, bool PERSISTENT, typename T = int, int N = -1, typename F = Unit_Prod<typename ActedMonoid::Monoid_X>>
struct Dynamic_Lazy_SegTree {
    using AM = ActedMonoid;
    using MX = ActedMonoid::Monoid_X;
    using MA = ActedMonoid::Monoid_A;
    using X = MX::value_type;
    using A = MA::value_type;

    struct Node {
        Node *l, *r;
        X x;
        A lazy;
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    T L, R;
    F default_prod;
    Dynamic_Lazy_SegTree(T L, T R, F default_prod = {}): L(L), R(R), default_prod(default_prod) {}
    np new_node(const X& x) { return pool.new_node({nullptr, nullptr, x, MA::unit()}); }
    np new_node(T l, T r) { return new_node(default_prod(l, r)); }
    np new_node() { return new_node(L, R); }
    np new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        return new_node([&](T i) { return a[i]; });
    }
    np new_node(std::invocable<T> auto&& f) {
        auto dfs = [&](auto&& dfs, T l, T r) -> np {
            if (l == r)
                return nullptr;
            if (l + 1 == r)
                return new_node(f(l));
            T m = (l + r) >> 1;
            np lr = dfs(dfs, l, m), rr = dfs(dfs, m, r);
            X x = MX::op(lr->x, rr->x);
            np u = new_node(x);
            u->l = lr, u->r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    np set(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> np {
            if (l + 1 == r) {
                u = copy_node(u);
                u->x = x;
                u->lazy = MA::unit();
                return u;
            }
            pushdown(u, l, r);
            T m = (l + r) >> 1;
            if (!u->l)
                u->l = new_node(l, m);
            if (!u->r)
                u->r = new_node(m, r);
            u = copy_node(u);
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            u->x = MX::op(u->l->x, u->r->x);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    np multiply(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> np {
            if (l + 1 == r) {
                u = copy_node(u);
                u->x = MX::op(u->x, x);
                u->lazy = MA::unit();
                return u;
            }
            pushdown(u, l, r);
            T m = (l + r) >> 1;
            if (!u->l)
                u->l = new_node(l, m);
            if (!u->r)
                u->r = new_node(m, r);
            u = copy_node(u);
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            u->x = MX::op(u->l->x, u->r->x);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    X prod(np u, T l, T r) {
        if (!u || l == r)
            return MX::unit();
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r, T ql, T qr, A lazy) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return;
            if (!u) {
                x = MX::op(x, AM::act(default_prod(ql, qr), lazy, qr - ql));
                return;
            }
            if (ql == l && qr == r) {
                x = MX::op(x, AM::act(u->x, lazy, r - l));
                return;
            }
            T m = (l + r) >> 1;
            lazy = MA::op(u->lazy, lazy);
            dfs(dfs, u->l, l, m, ql, qr, lazy);
            dfs(dfs, u->r, m, r, ql, qr, lazy);
        };
        dfs(dfs, u, L, R, l, r, MA::unit());
        return x;
    }
    X prod_all(np u) { return prod(u, L, R); }
    np apply(np u, T l, T r, const A& a) {
        if (l == r)
            return u;
        auto dfs = [&](auto&& dfs, np u, T l, T r, T ql, T qr) {
            if (!u)
                u = new_node(l, r);
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return u;
            if (ql == l && qr == r) {
                u = copy_node(u);
                u->x = AM::act(u->x, a, r - l);
                u->lazy = MA::op(u->lazy, a);
                return u;
            }
            pushdown(u, l, r);
            T m = (l + r) >> 1;
            u = copy_node(u);
            u->l = dfs(dfs, u->l, l, m, ql, qr);
            u->r = dfs(dfs, u->r, m, r, ql, qr);
            u->x = MX::op(u->l->x, u->r->x);
            return u;
        };
        return dfs(dfs, u, L, R, l, r);
    }
    T max_right(np u, auto&& check, T ql) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
            if (!u)
                u = new_node(l, r);
            if (r <= ql)
                return R;
            if (ql <= l && check(MX::op(x, u->x))) {
                x = MX::op(x, u->x);
                return R;
            }
            if (l + 1 == r)
                return l;
            pushdown(u, l, r);
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    T min_left(np u, auto&& check, T qr) {
        X x = MX::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
            if (!u)
                u = new_node(l, r);
            if (qr <= l)
                return L;
            if (r <= qr && check(MX::op(u->x, x))) {
                x = MX::op(u->x, x);
                return L;
            }
            if (l + 1 == r)
                return r;
            pushdown(u, l, r);
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->r, m, r);
            if (k != L)
                return k;
            return dfs(dfs, u->l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    np copy_node(np u) {
        if (u && PERSISTENT)
            return pool.new_node(*u);
        return u;
    }
    void pushdown(np u, T l, T r) {
        if (u->lazy == MA::unit())
            return;
        T m = (l + r) >> 1;
        u->l = u->l ? copy_node(u->l) : new_node(l, m);
        u->l->x = AM::act(u->l->x, u->lazy, m - l);
        u->l->lazy = MA::op(u->l->lazy, u->lazy);
        u->r = u->r ? copy_node(u->r) : new_node(m, r);
        u->r->x = AM::act(u->r->x, u->lazy, r - m);
        u->r->lazy = MA::op(u->r->lazy, u->lazy);
        u->lazy = MA::unit();
    }
};