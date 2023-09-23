#pragma once

#include "template.hpp"
#include "mod_inverse.hpp"

template <typename T, typename U>
constexpr T power(T a, U b) {
    T ans = 1;
    for (; b; b >>= 1, a *= a)
        if (b & 1)
            ans *= a;
    return ans;
}

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
    using int_double_t = impl::make_double_width_t<int_type>;
    static constexpr int base_width = std::numeric_limits<int_type>::digits;

    constexpr explicit MontgomeryReduction(int_type mod): _mod(mod),
                                                          _mod_neg_inv(inv_base(~(mod - 1))),
                                                          _mbase((int_double_t(1) << base_width) % mod),
                                                          _mbase2(int_double_t(_mbase) * _mbase % mod),
                                                          _mbase3(int_double_t(_mbase2) * _mbase % mod) {}

    constexpr int_type mod() const { return _mod; }
    constexpr int_type mbase() const { return _mbase; }
    constexpr int_type mbase2() const { return _mbase2; }
    constexpr int_type mbase3() const { return _mbase3; }
    constexpr int_type reduce(int_double_t t) const {
        int_type m = int_type(t) * _mod_neg_inv;
        int_type r = (t + int_double_t(m) * _mod) >> base_width;
        return r;
    }
    constexpr int_type shrink(int_type x) const {
        return x >= _mod * 2 ? x - _mod * 2 : x;
    }
    constexpr int_type strict_shrink(int_type x) const {
        return x >= _mod ? x - _mod : x;
    }

private:
    int_type _mod, _mod_neg_inv, _mbase, _mbase2, _mbase3;

    static constexpr int_type inv_base(int_type x) {
        int_type y = 1;
        for (int i = 1; i < base_width; i *= 2)
            y *= int_type(2) - x * y;
        return y;
    }
};
template <typename Context>
class MontgomeryModInt {
public:
    using mint = MontgomeryModInt;
    using int_type = typename Context::int_type;
    using mr_type = typename Context::mr_type;
    using int_double_t = typename mr_type::int_double_t;

    constexpr MontgomeryModInt(): _val(0) {}

    constexpr MontgomeryModInt(Signed auto x) {
        using signed_int_type = std::make_signed_t<int_type>;
        signed_int_type v = x % signed_int_type(mr().mod());
        _val = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mr().mod() : v));
    }

    constexpr MontgomeryModInt(Unsigned auto x) {
        _val = mr().reduce(mr().mbase2() * int_double_t(x % mr().mod()));
    }
    constexpr int_type val() const {
        return mr().strict_shrink(mr().reduce(_val));
    }
    constexpr int_type residue() const {
        return mr().strict_shrink(_val);
    }
    static constexpr int_type mod() {
        return mr().mod();
    }
    constexpr mint& operator++() {
        _val = mr().shrink(_val + mr().mbase());
        return *this;
    }
    constexpr mint operator++(int) {
        mint ret = *this;
        ++*this;
        return ret;
    }
    constexpr mint operator+() const {
        return *this;
    }
    constexpr mint& operator+=(const mint& rhs) {
        _val = mr().shrink(_val + rhs._val);
        return *this;
    }
    friend constexpr mint operator+(mint lhs, const mint& rhs) {
        return lhs += rhs;
    }
    constexpr mint& operator--() {
        _val = mr().shrink(_val + mr().mod() - mr().mbase());
        return *this;
    }
    constexpr mint operator--(int) {
        mint ret = *this;
        --(*this);
        return ret;
    }
    constexpr mint operator-() const {
        return from_raw(_val == 0 ? 0 : mr().mod() * 2 - _val);
    }
    constexpr mint& operator-=(const mint& rhs) {
        _val = mr().shrink(_val + mr().mod() * 2 - rhs._val);
        return *this;
    }
    friend constexpr mint operator-(mint lhs, const mint& rhs) {
        return lhs -= rhs;
    }
    constexpr mint& operator*=(const mint& rhs) {
        _val = mr().reduce(int_double_t(_val) * rhs._val);
        return *this;
    }
    friend constexpr mint operator*(mint lhs, const mint& rhs) {
        return lhs *= rhs;
    }
    constexpr mint inv() const {
        return from_raw(mr().reduce(int_double_t(mr().mbase3()) * mod_inverse(_val, mr().mod())));
    }
    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    friend constexpr mint operator/(mint lhs, const mint& rhs) {
        return lhs /= rhs;
    }
    constexpr bool operator==(const mint& rhs) const { return mr().strict_shrink(_val) == mr().strict_shrink(rhs._val); }

    static constexpr mint from_raw(int_type x) {
        mint ret;
        ret._val = x;
        return ret;
    }
#ifdef FASTIO
    void read(IO& io) {
        static int_type x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const {
        io.write(val());
    }
#endif
private:
    int_type _val;

    static constexpr const mr_type& mr() {
        return Context::montgomery_reduction();
    }
};
template <std::unsigned_integral T, T Mod>
class StaticMontgomeryReductionContext {
public:
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;
    static_assert(Mod % 2 == 1 && Mod <= std::numeric_limits<int_type>::max() / 4);

    static constexpr const mr_type& montgomery_reduction() {
        return _reduction;
    }

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
        assert(mod % 2 == 1 && mod <= std::numeric_limits<T>::max() / 4);
        _reduction_env.emplace_back(mod);
        return Guard();
    }
    static constexpr const mr_type& montgomery_reduction() {
        return _reduction_env.back();
    }

private:
    static inline std::vector<mr_type> _reduction_env;
};

template <u32 Mod>
using MMInt = MontgomeryModInt<StaticMontgomeryReductionContext<u32, Mod>>;
template <u64 Mod>
using MMInt64 = MontgomeryModInt<StaticMontgomeryReductionContext<u64, Mod>>;

using MMInt998244353 = MontgomeryModInt<StaticMontgomeryReductionContext<u32, 998244353>>;
using MMInt1000000007 = MontgomeryModInt<StaticMontgomeryReductionContext<u32, 1000000007>>;
