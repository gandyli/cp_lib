#pragma once
#include "template.hpp"

template <typename T>
struct Vector_Space {
    Vec<T> a;
    Vector_Space() = default;
    Vector_Space(const Vec<T>& a): a(a) { reduce(); }

    bool insert(T x) {
        foreach (a, a) {
            if (!x)
                break;
            chkmin(x, x ^ a);
        }
        if (x) {
            a.eb(x);
            return true;
        }
        return false;
    }
    bool contains(T x) const {
        foreach (a, a) {
            if (!x)
                return true;
            chkmin(x, x ^ a);
        }
        return false;
    }
    T max(T x = 0) const {
        T r = x;
        foreach (a, a)
            chkmax(r, r ^ a);
        return r;
    }
    T min(T x = 0) const {
        T r = x;
        foreach (a, a)
            chkmin(r, r ^ a);
        return r;
    }
    void reduce() {
        Vector_Space<T> b;
        foreach (a, a)
            b.insert(a);
        a = std::move(b.a);
    }
    void size() const { return a.size(); }
    void clear() { a.clear(); }

    static Vector_Space merge(const Vector_Space& a, const Vector_Space& b) {
        Vector_Space c;
        if (len(a) < len(b)) {
            c = b;
            foreach (a, a)
                c.insert(a);
        }
        else {
            c = a;
            foreach (a, b)
                c.insert(a);
        }
        return c;
    }
};