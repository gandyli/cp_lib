#pragma once
#include "template.hpp"

template <typename T, int N>
struct Memory_Pool {
    T* pool = new T[N];
    int id = 0;
    T* new_node(T x = {}) {
        pool[id] = x;
        return &pool[id++];
    }
    int new_node_id(T x = {}) {
        pool[id] = x;
        return id++;
    }
    T& operator[](int i) { return pool[i]; }
    const T& operator[](int i) const { return pool[i]; }
};
template <typename T>
struct Memory_Pool<T, -1> {
    std::deque<T> pool;
    T* new_node(T x = {}) {
        pool.eb(std::move(x));
        return &pool.eb(std::move(x));
    }
    int new_node_id(T x = {}) {
        pool.eb(std::move(x));
        return len(pool) - 1;
    }
    T& operator[](int i) { return pool[i]; }
    const T& operator[](int i) const { return pool[i]; }
};