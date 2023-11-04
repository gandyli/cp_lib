#pragma once
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

namespace impl {
    template <typename T>
    struct make_double_width {};
    template <>
    struct make_double_width<u8> {
        using type = u16;
    };
    template <>
    struct make_double_width<u16> {
        using type = u32;
    };
    template <>
    struct make_double_width<u32> {
        using type = u64;
    };
    template <>
    struct make_double_width<u64> {
        using type = u128;
    };
    template <typename T>
    using make_double_width_t = make_double_width<T>::type;
} // namespace impl
template <typename T>
class MontgomeryReduction {
public:
    using int_type = T;
    using int_double_t = impl::make_double_width_t<T>;
    static constexpr int base_width = std::numeric_limits<T>::digits;

    constexpr explicit MontgomeryReduction(T mod): _mod(mod),
                                                   _mod_neg_inv(inv_base(~(mod - 1))),
                                                   _mbase((int_double_t(1) << base_width) % mod),
                                                   _mbase2(int_double_t(_mbase) * _mbase % mod),
                                                   _mbase3(int_double_t(_mbase2) * _mbase % mod) {}

    constexpr T mod() const { return _mod; }
    constexpr T mbase() const { return _mbase; }
    constexpr T mbase2() const { return _mbase2; }
    constexpr T mbase3() const { return _mbase3; }
    constexpr T reduce(int_double_t t) const { return (t + int_double_t(T(t) * _mod_neg_inv) * _mod) >> base_width; }
    constexpr T shrink(T x) const { return x >= _mod * 2 ? x - _mod * 2 : x; }
    constexpr T strict_shrink(T x) const { return x >= _mod ? x - _mod : x; }
    static constexpr T inv_base(T x) {
        T y = 1;
        for (int i = 1; i < base_width; i *= 2)
            y *= 2 - x * y;
        return y;
    }

private:
    T _mod, _mod_neg_inv, _mbase, _mbase2, _mbase3;
};
template <typename Context>
class MontgomeryModInt {
public:
    using mint = MontgomeryModInt;
    using int_type = Context::int_type;
    using mr_type = Context::mr_type;
    using int_double_t = mr_type::int_double_t;

    constexpr MontgomeryModInt() = default;
    constexpr MontgomeryModInt(Signed auto y) {
        using S = std::make_signed_t<int_type>;
        S v = y % S(mr().mod());
        x = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mr().mod() : v));
    }
    constexpr MontgomeryModInt(Unsigned auto y) { x = mr().reduce(mr().mbase2() * int_double_t(y % mr().mod())); }
    constexpr int_type val() const { return mr().strict_shrink(mr().reduce(x)); }
    constexpr int_type residue() const { return mr().strict_shrink(x); }
    static constexpr int_type mod() { return mr().mod(); }
    constexpr mint& operator++() {
        x = mr().shrink(x + mr().mbase());
        return *this;
    }
    constexpr mint operator++(int) {
        mint ret = *this;
        ++*this;
        return ret;
    }
    constexpr mint& operator+=(const mint& rhs) {
        x = mr().shrink(x + rhs.x);
        return *this;
    }
    constexpr mint& operator--() {
        x = mr().shrink(x + mr().mod() - mr().mbase());
        return *this;
    }
    constexpr mint operator--(int) {
        mint ret = *this;
        --*this;
        return ret;
    }
    constexpr mint& operator-=(const mint& rhs) {
        x = mr().shrink(x + mr().mod() * 2 - rhs.x);
        return *this;
    }
    constexpr mint& operator*=(const mint& rhs) {
        x = mr().reduce(int_double_t(x) * rhs.x);
        return *this;
    }
    constexpr mint inv() const { return from_raw(mr().reduce(int_double_t(mr().mbase3()) * mod_inverse(x, mr().mod()))); }
    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(!x ? 0 : mr().mod() * 2 - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    constexpr bool operator==(const mint& rhs) const { return mr().strict_shrink(x) == mr().strict_shrink(rhs.x); }

    static constexpr mint from_raw(int_type x) {
        mint ret;
        ret.x = x;
        return ret;
    }
    constexpr int_type raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static int_type x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(val()); }
#endif
    static constexpr const mr_type& mr() { return Context::montgomery_reduction(); }

private:
    int_type x{};
};
template <std::unsigned_integral T, T Mod>
class StaticMontgomeryReductionContext {
public:
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;
    static_assert(Mod % 2 && Mod <= std::numeric_limits<T>::max() / 4);
    static constexpr const mr_type& montgomery_reduction() { return _reduction; }

private:
    static constexpr auto _reduction = mr_type(Mod);
};

template <std::unsigned_integral T>
class DynamicMontgomeryReductionContext {
public:
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;

    class Guard {
    public:
        Guard(const Guard&) = delete;
        Guard(Guard&&) = delete;
        Guard& operator=(const Guard&) = delete;
        Guard& operator=(Guard&&) = delete;
        ~Guard() { _reduction_env.pop_back(); }

    private:
        friend DynamicMontgomeryReductionContext;
        Guard() = default;
    };
    [[nodiscard]] static Guard set_mod(T mod) {
        ASSERT(mod % 2 == 1 && mod <= std::numeric_limits<T>::max() / 4);
        _reduction_env.eb(mod);
        return {};
    }
    static constexpr const mr_type& montgomery_reduction() { return _reduction_env.back(); }

private:
    static inline Vec<mr_type> _reduction_env;
};

template <u32 Mod>
using MMInt = MontgomeryModInt<StaticMontgomeryReductionContext<u32, Mod>>;
template <u64 Mod>
using MMInt64 = MontgomeryModInt<StaticMontgomeryReductionContext<u64, Mod>>;

using MMInt998244353 = MMInt<998244353>;
using MMInt1000000007 = MMInt<1000000007>;

#define SetMod(T, mod)                                \
    using ctx = DynamicMontgomeryReductionContext<T>; \
    auto _guard = ctx::set_mod(mod);                  \
    using mint = MontgomeryModInt<ctx>
