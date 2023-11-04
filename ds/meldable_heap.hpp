#pragma once
#include "template.hpp"

template <typename T, bool PERSISTENT, int N, typename Compare = std::less<>>
struct Meldable_Heap: public Compare {
    struct Node {
        Node *l, *r;
        T x;
        int s;
    }* pool{new Node[N]};
    int id{};

    Meldable_Heap() = default;
    Meldable_Heap(const Compare& cmp): Compare(cmp) {}
    Node* new_node(const T& x) {
        pool[id] = {nullptr, nullptr, x, 1};
        return &pool[id++];
    }
    Node* copy_node(Node* a) {
        if (!a || !PERSISTENT)
            return a;
        Node* b = new_node(a->x);
        b->s = a->s;
        b->l = a->l;
        b->r = a->r;
        return b;
    }
    Node* meld(Node* a, Node* b) {
        if (!a)
            return b;
        if (!b)
            return a;
        a = copy_node(a);
        b = copy_node(b);
        if ((*this)(b->x, a->x))
            swap(a, b);
        a->r = a->r ? meld(a->r, b) : b;
        if (!a->l || a->l->s < a->r->s)
            swap(a->l, a->r);
        a->s = (a->r ? a->r->s : 0) + 1;
        return a;
    }
    Node* push(Node* a, const T& x) { return meld(a, new_node(x)); }
    Node* pop(Node* a) { return meld(a->l, a->r); }
    static T top(Node* a) { return a->x; }
    static Vec<T> get_all(Node* a) {
        Vec<T> r;
        auto dfs = [&](auto&& dfs, Node* a) {
            if (!a)
                return;
            r.eb(a->x);
            dfs(dfs, a->l);
            dfs(dfs, a->r);
        };
        dfs(dfs, a);
        return r;
    }
};