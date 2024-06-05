#pragma once
#include "utility/make_double_width.hpp"

template <typename T>
struct MontgomeryReduction {
    using int_type = T;
    using int_double_t = make_double_width_t<T>;
    static constexpr int base_width = std::numeric_limits<T>::digits;

    constexpr explicit MontgomeryReduction(T mod): _mod(mod), _mod2(mod * 2), _mod_neg_inv(inv_base(-mod)), _mbase((int_double_t(1) << base_width) % mod), _mbase2(int_double_t(_mbase) * _mbase % mod), _mbase3(int_double_t(_mbase2) * _mbase % mod) {}

    constexpr T mod() const { return _mod; }
    constexpr T mod2() const { return _mod2; }
    constexpr T mod_neg_inv() const { return _mod_neg_inv; }
    constexpr T mbase() const { return _mbase; }
    constexpr T mbase2() const { return _mbase2; }
    constexpr T mbase3() const { return _mbase3; }
    constexpr T reduce(int_double_t t) const { return T((t + int_double_t(T(t) * _mod_neg_inv) * _mod) >> base_width); }
    constexpr T shrink(T x) const { return x >= _mod2 ? x - _mod2 : x; }
    constexpr T strict_shrink(T x) const { return x >= _mod ? x - _mod : x; }
    static constexpr T inv_base(T x) {
        T y = 1;
        for (int i = 1; i < base_width; i *= 2)
            y *= 2 - x * y;
        return y;
    }

private:
    T _mod, _mod2, _mod_neg_inv, _mbase, _mbase2, _mbase3;
};