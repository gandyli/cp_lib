#pragma once
#include "modint/barrett_reduction.hpp"
#include "modint/modint_common.hpp"

template <typename Context>
struct BarrettModInt {
    using mint = BarrettModInt;
    using int_type = Context::int_type;
    using br_type = Context::br_type;

    constexpr BarrettModInt() = default;
    constexpr BarrettModInt(Signed auto y) {
        using S = std::make_signed_t<int_type>;
        S v = y % S(mod());
        x = v < 0 ? v + mod() : v;
    }
    constexpr BarrettModInt(Unsigned auto y) { x = y % mod(); }
    constexpr int_type val() const { return x; }
    static constexpr int_type mod() { return br().mod(); }
    constexpr mint& operator++() {
        x++;
        if (x == mod())
            x = 0;
        return *this;
    }
    constexpr mint operator++(int) {
        mint r = *this;
        ++*this;
        return r;
    }
    constexpr mint& operator+=(const mint& p) {
        x += p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint& operator--() {
        if (x == 0)
            x = mod();
        x--;
        return *this;
    }
    constexpr mint operator--(int) {
        mint r = *this;
        --*this;
        return r;
    }
    constexpr mint& operator-=(const mint& p) {
        x += mod() - p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint& operator*=(const mint& p) {
        x = br().mul(x, p.x);
        return *this;
    }
    constexpr mint inv() const { return from_raw(mod_inverse(x, mod())); }
    constexpr mint& operator/=(const mint& p) { return *this *= p.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(!x ? 0 : mod() - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend constexpr auto operator<=>(const mint&, const mint&) = default;

    static constexpr mint from_raw(int_type x) {
        mint r;
        r.x = x;
        return r;
    }
    constexpr int_type raw() const { return x; }
#ifdef FASTIO
    void read(auto& io) {
        static int_type x;
        io.read(x);
        *this = x;
    }
    void write(auto& io) const { io.write(val()); }
#endif
    static constexpr const br_type& br() { return Context::barrett_reduction(); }

private:
    int_type x{};
};
template <std::unsigned_integral T, T Mod>
requires (0 < Mod && Mod <= std::numeric_limits<T>::max() / 4)
struct StaticBarrettReductionContext {
    using int_type = T;
    using br_type = BarrettReduction<T>;
    static constexpr const br_type& barrett_reduction() { return _reduction; }

private:
    static constexpr auto _reduction = br_type(Mod);
};
template <std::unsigned_integral T>
struct DynamicBarrettReductionContext {
    using int_type = T;
    using br_type = BarrettReduction<T>;

    struct Guard {
        Guard(const Guard&) = delete;
        Guard& operator=(const Guard&) = delete;
        ~Guard() { _reduction_env.pop_back(); }

    private:
        friend DynamicBarrettReductionContext;
        Guard() = default;
    };
    [[nodiscard]] static Guard set_mod(T mod) {
        ASSERT(0 < mod && mod <= std::numeric_limits<T>::max() / 4);
        _reduction_env.eb(mod);
        return {};
    }
    static constexpr const br_type& barrett_reduction() { return _reduction_env.back(); }

private:
    static inline vc<br_type> _reduction_env;
};
template <u32 Mod>
using BMInt = BarrettModInt<StaticBarrettReductionContext<u32, Mod>>;
template <u64 Mod>
using BMInt64 = BarrettModInt<StaticBarrettReductionContext<u64, Mod>>;

#define SetBMod(T, mod)                            \
    using ctx = DynamicBarrettReductionContext<T>; \
    auto _guard = ctx::set_mod(mod);               \
    using mint = BarrettModInt<ctx>
