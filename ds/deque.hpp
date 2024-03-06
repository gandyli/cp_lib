#pragma once
#include "template.hpp"

template <typename T, int N, bool MID = true>
struct Deque {
    T* a;
    int l, r;
    Deque(): a(new T[N]) {
        if constexpr (MID)
            l = r = N / 2;
        else
            l = r = 0;
    }
    T* begin() { return a + l; }
    T* end() { return a + r; }
    const T* begin() const { return a + l; }
    const T* end() const { return a + r; }
    void clear() { l = r = 0; }
    void push_back(T x) { a[r++] = std::move(x); }
    T& emplace_back(auto&&... args) { return *new (a + r++) T(FORWARD(args)...); }
    void pop_back() { r--; }
    T& back() { return a[r - 1]; }
    const T& back() const { return a[r - 1]; }
    void push_front(T x) { a[--l] = std::move(x); }
    T& emplace_front(auto&&... args) { return *new (a + --l) T(FORWARD(args)...); }
    void pop_front() { l++; }
    T& front() { return a[l]; }
    const T& front() const { return a[l]; }
    T& operator[](int i) { return a[l + i]; }
    const T& operator[](int i) const { return a[l + i]; }
    int size() const { return r - l; }
    bool empty() const { return l == r; }
    ~Deque() { delete[] a; }
};