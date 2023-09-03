#pragma once

#include "template.hpp"

static_assert(__cplusplus >= 201703L);

template <typename T, typename U>
T power(T a, U b) {
    T ans = 1;
    for (; b; b >>= 1, a *= a)
        if (b & 1)
            ans *= a;
    return ans;
}

namespace impl {
    template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
    std::tuple<typename std::make_signed<T>::type, typename std::make_signed<T>::type, T> bezout(T x, T y) {
        bool swap = x < y;
        if (swap)
            std::swap(x, y);
        if (y == 0) {
            if (x == 0)
                return {0, 0, 0};
            if (swap)
                return {0, 1, x};
            return {1, 0, x};
        }
        using S = typename std::make_signed<T>::type;
        S s0 = 1, s1 = 0, t0 = 0, t1 = 1;
        loop {
            auto qr = divmod(x, y);
            auto q = qr.first, r = qr.second;
            if (r == 0) {
                if (swap)
                    return {t1, s1, y};
                return {s1, t1, y};
            }
            S s2 = s0 - S(q) * s1, t2 = t0 - S(q) * t1;
            x = y;
            y = r;
            s0 = s1;
            s1 = s2;
            t0 = t1;
            t1 = t2;
        }
    }
    template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
    T mod_inverse(T x, T m) {
        using S = typename std::make_signed<T>::type;
        S s, t;
        T g;
        std::tie(s, t, g) = bezout(x, m);
        assert(g == 1);
        return s < 0 ? T(s) + m : s;
    }
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
    using make_double_width_t = typename make_double_width<T>::type;
}  // namespace impl
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
    int_type reduce(int_double_t t) const {
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

    MontgomeryModInt(): _val(0) {}

    template <typename T, typename std::enable_if<std::is_signed<T>::value>::type* = nullptr>
    MontgomeryModInt(T x) {
        using signed_int_type = typename std::make_signed<int_type>::type;
        signed_int_type v = x % signed_int_type(mr().mod());
        _val = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mr().mod() : v));
    }

    template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
    MontgomeryModInt(T x) {
        _val = mr().reduce(mr().mbase2() * int_double_t(x % mr().mod()));
    }
    int_type val() const {
        return mr().strict_shrink(mr().reduce(_val));
    }
    int_type residue() const {
        return mr().strict_shrink(_val);
    }
    static int_type mod() {
        return mr().mod();
    }
    mint& operator++() {
        _val = mr().shrink(_val + mr().mbase());
        return *this;
    }
    mint operator++(int) {
        mint ret = *this;
        ++*this;
        return ret;
    }
    mint operator+() const {
        return *this;
    }
    mint& operator+=(const mint& rhs) {
        _val = mr().shrink(_val + rhs._val);
        return *this;
    }
    friend mint operator+(mint lhs, const mint& rhs) {
        return lhs += rhs;
    }
    mint& operator--() {
        _val = mr().shrink(_val + mr().mod() - mr().mbase());
        return *this;
    }
    mint operator--(int) {
        mint ret = *this;
        --(*this);
        return ret;
    }
    mint operator-() const {
        return from_raw(_val == 0 ? 0 : mr().mod() * 2 - _val);
    }
    mint& operator-=(const mint& rhs) {
        _val = mr().shrink(_val + mr().mod() * 2 - rhs._val);
        return *this;
    }
    friend mint operator-(mint lhs, const mint& rhs) {
        return lhs -= rhs;
    }
    mint& operator*=(const mint& rhs) {
        _val = mr().reduce(int_double_t(_val) * rhs._val);
        return *this;
    }
    friend mint operator*(mint lhs, const mint& rhs) {
        return lhs *= rhs;
    }
    mint inv() const {
        return from_raw(mr().reduce(int_double_t(mr().mbase3()) * impl::mod_inverse(_val, mr().mod())));
    }
    mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    friend mint operator/(mint lhs, const mint& rhs) {
        return lhs /= rhs;
    }
    bool operator==(const mint& rhs) const { return mr().strict_shrink(_val) == mr().strict_shrink(rhs._val); }
    bool operator!=(const mint& rhs) const { return !(*this == rhs); }

private:
    int_type _val;

    static mint from_raw(int_type x) {
        mint ret;
        ret._val = x;
        return ret;
    }

    static const mr_type& mr() {
        return Context::montgomery_reduction();
    }
};
template <typename T, T Mod, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
class StaticMontgomeryReductionContext {
public:
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;
    static_assert(Mod % 2 == 1 && Mod <= std::numeric_limits<int_type>::max() / 4, "");

    static constexpr const mr_type& montgomery_reduction() {
        return _reduction;
    }

private:
    static constexpr auto _reduction = mr_type(Mod);
};

template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
class DynamicMontgomeryReductionContext {
public:
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;

    class Guard {
    public:
        // Guard(const Guard&) = delete;
        // Guard(Guard&&) = delete;
        // Guard& operator=(const Guard&) = delete;
        // Guard& operator=(Guard&&) = delete;
        ~Guard() { _reduction_env.pop_back(); }

    private:
        friend DynamicMontgomeryReductionContext;
        Guard() {}
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
    static std::vector<mr_type> _reduction_env;
};

template <u32 Mod>
using MMInt = MontgomeryModInt<StaticMontgomeryReductionContext<u32, Mod>>;
template <u64 Mod>
using MMInt64 = MontgomeryModInt<StaticMontgomeryReductionContext<u64, Mod>>;

using MMInt998244353 = MontgomeryModInt<StaticMontgomeryReductionContext<u32, 998244353>>;
using MMInt1000000007 = MontgomeryModInt<StaticMontgomeryReductionContext<u32, 1000000007>>;
