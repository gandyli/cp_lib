#pragma once
#include "template.hpp"

template <typename Monoid>
struct Deafult_Prod_Unit {
    constexpr auto operator()(i64, i64) const { return Monoid::unit(); }
};
template <typename Monoid, bool PERSISTENT, int N, typename F = Deafult_Prod_Unit<Monoid>>
struct Dynamic_SegTree {
    using M = Monoid;
    using X = M::value_type;

    struct Node {
        Node *l, *r;
        X x;
    }* pool{new Node[N]};
    int id{};

    i64 L, R;
    F default_prod;
    Dynamic_SegTree(i64 L, i64 R, F default_prod = {}): L(L), R(R), default_prod(default_prod) {}
    Node* new_node(const X& x) {
        pool[id] = {nullptr, nullptr, x};
        return &pool[id++];
    }
    Node* new_node(i64 l, i64 r) {
        return new_node(default_prod(l, r));
    }
    Node* new_node() { return new_node(L, R); }
    Node* new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        auto dfs = [&](auto&& dfs, i64 l, i64 r) -> Node* {
            if (l == r)
                return nullptr;
            if (l + 1 == r)
                return new_node(a[l]);
            i64 m = (l + r) / 2;
            Node *lr = dfs(dfs, l, m), *rr = dfs(dfs, m, r);
            X x = M::op(lr->x, rr->x);
            Node* u = new_node(x);
            u->l = lr, u->r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    X prod(Node* u, i64 l, i64 r) {
        if (!u || l == r)
            return M::unit();
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r, i64 ql, i64 qr) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return;
            if (!u) {
                x = M::op(x, default_prod(ql, qr));
                return;
            }
            if (ql == l && qr == r) {
                x = M::op(x, u->x);
                return;
            }
            i64 m = (l + r) / 2;
            dfs(dfs, u->l, l, m, ql, qr);
            dfs(dfs, u->r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    Node* set(Node* u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> Node* {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = x;
                return u;
            }
            i64 m = (l + r) / 2;
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            X xl = u->l ? u->l->x : default_prod(l, m);
            X xr = u->r ? u->r->x : default_prod(m, r);
            u->x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    Node* multiply(Node* u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> Node* {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = M::op(u->x, x);
                return u;
            }
            i64 m = (l + r) / 2;
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            X xl = u->l ? u->l->x : default_prod(l, m);
            X xr = u->r ? u->r->x : default_prod(m, r);
            u->x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    i64 max_right(Node* u, auto&& check, i64 ql) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (r <= ql)
                return R;
            if (ql <= l && check(M::op(x, u->x))) {
                x = M::op(x, u->x);
                return R;
            }
            if (l + 1 == r)
                return l;
            i64 m = (l + r) / 2;
            i64 k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    i64 min_left(Node* u, auto&& check, i64 qr) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, Node* u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (qr <= l)
                return L;
            if (r <= qr && check(M::op(u->x, x))) {
                x = M::op(u->x, x);
                return L;
            }
            if (l + 1 == r)
                return r;
            i64 m = (l + r) / 2;
            i64 k = dfs(dfs, u->r, m, r);
            if (k != L)
                return k;
            return dfs(dfs, u->l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    Node* copy_node(Node* u) {
        if (u && PERSISTENT) {
            pool[id] = *u;
            return &pool[id++];
        }
        return u;
    }
};