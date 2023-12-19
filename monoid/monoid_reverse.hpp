#pragma once

template <typename M>
struct Monoid_Reverse {
    using value_type = M::value_type;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return M::op(b, a); }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return M::unit(); }
    static constexpr bool commute = M::commute;
};