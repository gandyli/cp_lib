#pragma once
#include "modint/montgomery_reduction.hpp"
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

template <typename Context>
struct MontgomeryModInt {
    using mint = MontgomeryModInt;
    using int_type = Context::int_type;
    using mr_type = Context::mr_type;
    using int_double_t = mr_type::int_double_t;

    constexpr MontgomeryModInt() = default;
    constexpr MontgomeryModInt(Signed auto y) {
        using S = std::make_signed_t<int_type>;
        S v = y % S(mod());
        x = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mod() : v));
    }
    constexpr MontgomeryModInt(Unsigned auto y) { x = mr().reduce(mr().mbase2() * int_double_t(y % mod())); }
    constexpr int_type val() const { return mr().strict_shrink(mr().reduce(x)); }
    constexpr int_type residue() const { return mr().strict_shrink(x); }
    static constexpr int_type mod() { return mr().mod(); }
    constexpr mint& operator++() {
        x = mr().shrink(x + mr().mbase());
        return *this;
    }
    constexpr mint operator++(int) {
        mint r = *this;
        ++*this;
        return r;
    }
    constexpr mint& operator+=(const mint& rhs) {
        x = mr().shrink(x + rhs.x);
        return *this;
    }
    constexpr mint& operator--() {
        x = mr().shrink(x + mod() - mr().mbase());
        return *this;
    }
    constexpr mint operator--(int) {
        mint r = *this;
        --*this;
        return r;
    }
    constexpr mint& operator-=(const mint& rhs) {
        x = mr().shrink(x + mod() * 2 - rhs.x);
        return *this;
    }
    constexpr mint& operator*=(const mint& rhs) {
        x = mr().reduce(int_double_t(x) * rhs.x);
        return *this;
    }
    constexpr mint inv() const { return from_raw(mr().reduce(int_double_t(mr().mbase3()) * mod_inverse(x, mod()))); }
    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(!x ? 0 : mod() * 2 - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) { return mr().strict_shrink(lhs.x) == mr().strict_shrink(rhs.x); }
    friend constexpr auto operator<=>(const mint& lhs, const mint& rhs) { return mr().strict_shrink(lhs.x) <=> mr().strict_shrink(rhs.x); }

    static constexpr mint from_raw(int_type x) {
        mint r;
        r.x = x;
        return r;
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
requires (Mod % 2 == 1 && Mod <= std::numeric_limits<T>::max() / 4)
struct StaticMontgomeryReductionContext {
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;
    static constexpr const mr_type& montgomery_reduction() { return _reduction; }

private:
    static constexpr auto _reduction = mr_type(Mod);
};

template <std::unsigned_integral T>
struct DynamicMontgomeryReductionContext {
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;

    struct Guard {
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

#define SetMMod(T, mod)                               \
    using ctx = DynamicMontgomeryReductionContext<T>; \
    auto _guard = ctx::set_mod(mod);                  \
    using mint = MontgomeryModInt<ctx>
