#pragma once
#include "template.hpp"

template <typename T, bool PERSISTENT, int N>
struct Dynamic_Array {
    struct Node {
        T x;
        Node* ch[2];
    }* pool{new Node[N]};
    int id{};

    T x0;
    Dynamic_Array(T x0 = {}): x0(std::move(x0)) {}

    Node* new_node() {
        pool[id] = {x0};
        return &pool[id++];
    }
    Node* new_node(const vc<T>& a) {
        Node* u = new_node();
        _for (i, len(a))
            u = set(u, i, a[i], false);
        return u;
    }
    T get(Node* u, int i) const {
        if (!u)
            return x0;
        if (i == 0)
            return u->x;
        return get(u->ch[i & 1], (i - 1) >> 1);
    }
    Node* set(Node* u, int i, T x, bool copy = true) {
        if (!u)
            u = new_node();
        else if (copy && PERSISTENT) {
            pool[id].x = u->x;
            pool[id].ch[0] = u->ch[0];
            pool[id].ch[1] = u->ch[1];
            u = &pool[id++];
        }
        if (i == 0) {
            u->x = std::move(x);
            return u;
        }
        u->ch[i & 1] = set(u->ch[i & 1], (i - 1) >> 1, std::move(x), copy);
        return u;
    }
};