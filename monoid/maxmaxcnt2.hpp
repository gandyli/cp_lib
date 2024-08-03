#pragma once
#include "template.hpp"

template <typename T>
struct Monoid_MaxMaxcnt2 {
    struct X {
        T max, max2;
        T cnt, cnt2;
    };
    using value_type = X;
    static constexpr X op(const X& a, const X& b) {
        if (a.max == b.max) {
            if (a.max2 == b.max2)
                return {a.max, a.max2, a.cnt + b.cnt, a.cnt2 + b.cnt2};
            if (a.max2 > b.max2)
                return {a.max, a.max2, a.cnt + b.cnt, a.cnt2};
            return {a.max, b.max2, a.cnt + b.cnt, b.cnt2};
        }
        if (a.max > b.max) {
            if (a.max2 == b.max)
                return {a.max, a.max2, a.cnt, a.cnt2 + b.cnt};
            if (a.max2 > b.max)
                return {a.max, a.max2, a.cnt, a.cnt2};
            return {a.max, b.max, a.cnt, b.cnt};
        }
        if (b.max2 == a.max)
            return {b.max, b.max2, b.cnt, b.cnt2 + a.cnt};
        if (b.max2 > a.max)
            return {b.max, b.max2, b.cnt, b.cnt2};
        return {b.max, a.max, b.cnt, a.cnt};
    }
    static constexpr X from_element(auto&& x) { return {x, -inf<T>, 1, 0}; }
    static constexpr X unit() { return {-inf<T>, -inf<T>, 0, 0}; }
    static constexpr bool commute() { return true; }
};