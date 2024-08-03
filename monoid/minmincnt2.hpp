#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_MinMincnt2 {
    struct X {
        T min, min2;
        T cnt, cnt2;
    };
    using value_type = X;
    static constexpr X op(const X& a, const X& b) {
        if (a.min == b.min) {
            if (a.min2 == b.min2)
                return {a.min, a.min2, a.cnt + b.cnt, a.cnt2 + b.cnt2};
            if (a.min2 < b.min2)
                return {a.min, a.min2, a.cnt + b.cnt, a.cnt2};
            return {a.min, b.min2, a.cnt + b.cnt, b.cnt2};
        }
        if (a.min < b.min) {
            if (a.min2 == b.min)
                return {a.min, a.min2, a.cnt, a.cnt2 + b.cnt};
            if (a.min2 < b.min)
                return {a.min, a.min2, a.cnt, a.cnt2};
            return {a.min, b.min, a.cnt, b.cnt};
        }
        if (b.min2 == a.min)
            return {b.min, b.min2, b.cnt, b.cnt2 + a.cnt};
        if (b.min2 < a.min)
            return {b.min, b.min2, b.cnt, b.cnt2};
        return {b.min, a.min, b.cnt, a.cnt};
    }
    static constexpr X from_element(auto&& x) { return {x, inf<T>, 1, 0}; }
    static constexpr X unit() { return {inf<T>, inf<T>, 0, 0}; }
    static constexpr bool commute() { return true; }
};