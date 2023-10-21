#include "../template.hpp"
#include "../math/mod_inverse.hpp"

struct MMInt61 {
    using mint = MMInt61;
    using int_type = u64;
    constexpr MMInt61() = default;
    constexpr MMInt61(Signed auto x) {
        x %= mod();
        _val = x < 0 ? x + mod() : x;
    }
    constexpr MMInt61(Unsigned auto x) {
        _val = x % mod();
    }
    constexpr int_type val() const {
        return _val;
    }
    static constexpr int_type mod() {
        return (1ULL << 61) - 1;
    }
    constexpr mint& operator++() {
        _val++;
        if (_val == mod())
            _val = 0;
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
        _val += rhs._val;
        if (_val >= mod())
            _val -= mod();
        return *this;
    }
    friend constexpr mint operator+(mint lhs, const mint& rhs) {
        return lhs += rhs;
    }
    constexpr mint& operator--() {
        if (_val == 0)
            _val = mod();
        _val--;
        return *this;
    }
    constexpr mint operator--(int) {
        mint ret = *this;
        --*this;
        return ret;
    }
    constexpr mint operator-() const {
        return from_raw(_val == 0 ? 0 : mod() - _val);
    }
    constexpr mint& operator-=(const mint& rhs) {
        _val += mod() - rhs._val;
        if (_val >= mod())
            _val -= mod();
        return *this;
    }
    friend constexpr mint operator-(mint lhs, const mint& rhs) {
        return lhs -= rhs;
    }
    constexpr mint& operator*=(const mint& rhs) {
        u128 t = u128(_val) * rhs._val;
        _val = u64(t >> 61) + u64(t & mod());
        if (_val >= mod())
            _val -= mod();
        return *this;
    }
    friend constexpr mint operator*(mint lhs, const mint& rhs) {
        return lhs *= rhs;
    }
    constexpr mint inv() const {
        return from_raw(mod_inverse(_val, mod()));
    }
    constexpr mint& operator/=(const mint& rhs) {
        return *this *= rhs.inv();
    }
    friend constexpr mint operator/(mint lhs, const mint& rhs) {
        return lhs /= rhs;
    }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._val == rhs._val;
    }
    static constexpr mint from_raw(u64 x) {
        mint ret;
        ret._val = x;
        return ret;
    }
    constexpr u64 raw() const { return _val; }
#ifdef FASTIO
    void read(IO& io) {
        static u64 x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const {
        io.write(val());
    }
#endif

private:
    u64 _val{};
};