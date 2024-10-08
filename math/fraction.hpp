#pragma once
#include "math/gcd.hpp"

template <typename T, typename U>
struct FractionBase {
    using R = FractionBase;
    using Key = T;
    T x{0}, y{1};
    FractionBase() = default;
    FractionBase(auto x): FractionBase(x, 1) {}
    FractionBase(auto _x, auto _y): x(_x), y(_y) {
        if (y < 0)
            x = -x, y = -y;
        if (y != 1) {
            T g = gcd(x, y);
            if (g != 0)
                x /= g, y /= g;
        }
    }
    static R from_raw(T _x, T _y) {
        R r;
        r.x = _x, r.y = _y;
        return r;
    }
    friend R operator+(R l, R r) {
        if (l.y == r.y)
            return {l.x + r.x, l.y};
        return {l.x * r.y + l.y * r.x, l.y * r.y};
    }
    friend R operator-(R l, R r) {
        if (l.y == r.y)
            return {l.x - r.x, l.y};
        return {l.x * r.y - l.y * r.x, l.y * r.y};
    }
    friend R operator*(R l, R r) { return {l.x * r.x, l.y * r.y}; }
    friend R operator/(R l, R r) { return {l.x * r.y, l.y * r.x}; }
    R& operator+=(R r) { return *this = *this + r; }
    R& operator-=(R r) { return *this = *this - r; }
    R& operator*=(R r) { return *this = *this * r; }
    R& operator/=(R r) { return *this = *this / r; }
    R operator-() const { return from_raw(-x, y); }
    friend bool operator==(const R&, const R&) = default;
    friend auto operator<=>(const R& l, const R& r) { return U{l.x} * r.y <=> U{l.y} * r.x; }
#ifdef FASTIO
    void read(auto& io) { io.read(x, y); }
    void write(auto& io) const {
        io.write(x);
        if (x != 0 && y != 1)
            io.write('/', y);
    }
#endif
    explicit operator bool() const { return !!x; }
    bool operator!() const { return !x; }
};
using Fraction = FractionBase<i64, i128>;