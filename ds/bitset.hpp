#pragma once
#include "template.hpp"

struct bitset {
    vc<u64> a;
    usize n;
    bitset(usize n = 0): a((n + 63) >> 6), n(n) {}
    bitset(std::string_view s): a((len(s) + 63) >> 6), n(len(s)) {
        _for (i, len(s))
            if (s[i] == '1')
                a[i >> 6] |= 1ULL << (i & 63);
    }
    usize size() const { return n; }
    struct reference {
        u64& x;
        usize i;
        reference(u64& x, usize i): x(x), i(i) {}
        operator bool() const { return x >> i & 1; }
        bool operator~() const { return !(x >> i & 1); }
        reference& operator=(bool b) {
            if (b)
                x |= 1ULL << i;
            else
                x &= ~(1ULL << i);
            return *this;
        }
        reference& operator=(const reference& r) { return *this = bool(r); } // NOLINT
        reference& flip() {
            x ^= 1ULL << i;
            return *this;
        }
    };
    reference operator[](usize i) { return {a[i >> 6], i & 63}; }
    bool operator[](usize i) const { return a[i >> 6] >> (i & 63) & 1; }
    bitset& operator&=(const bitset& b) {
        ASSERT(size() == b.size());
        _for (i, len(a))
            a[i] &= b.a[i];
        return *this;
    }
    bitset& operator|=(const bitset& b) {
        ASSERT(size() == b.size());
        _for (i, len(a))
            a[i] |= b.a[i];
        return *this;
    }
    bitset& operator^=(const bitset& b) {
        ASSERT(size() == b.size());
        _for (i, len(a))
            a[i] ^= b.a[i];
        return *this;
    }
    friend bitset operator&(bitset a, const bitset& b) { return a &= b; }
    friend bitset operator|(bitset a, const bitset& b) { return a |= b; }
    friend bitset operator^(bitset a, const bitset& b) { return a ^= b; }
    bitset operator~() const {
        bitset b = *this;
        b.flip();
        return b;
    }
    usize count() const {
        usize res = 0;
        _for (i, len(a))
            res += std::popcount(a[i]);
        return res;
    }
    int next(usize i) {
        if (i >= n)
            return n;
        usize j = i >> 6;
        u64 x = a[j] & ~0ULL << (i & 63);
        if (x)
            return j * 64 + std::countr_zero(x);
        while (++j < a.size())
            if (a[j])
                return j * 64 + std::countr_zero(a[j]);
        return n;
    }
    int prev(usize i) {
        chkmin(i, n - 1);
        if ((*this)[i])
            return i;
        usize j = i >> 6;
        u64 x = a[j] & ((1ULL << (i & 63)) - 1);
        if (x)
            return j * 64 + std::__lg(x);
        while (j--)
            if (x)
                return j * 64 + std::__lg(x);
        return -1;
    }
    void set(usize i) { (*this)[i] = true; }
    void reset(usize i) { (*this)[i] = false; }
    void flip(usize i) { (*this)[i].flip(); }
    void set() {
        _for (i, len(a) - 1)
            a[i] = ~0ULL;
        a.back() = (1ULL << (n & 63)) - 1;
    }
    void reset() { fill(a, 0); }
    void flip() {
        _for (i, len(a) - 1)
            a[i] = ~a[i];
        a.back() = ~a.back() & ((1ULL << (n & 63)) - 1);
    }
    bool any() const {
        _for (i, len(a))
            if (a[i])
                return true;
        return false;
    }
    bool none() const { return !any(); }
    static constexpr auto O = [] {
        std::array<u64, 256> O{};
        _for (i, 256)
            _for (j, 8)
                O[i] = O[i] << 8 | ((i >> (7 - j) & 1) ^ 48);
        return O;
    }();
#ifdef FASTIO
    void read(auto& io) {
        static str s;
        io.read(s);
        *this = bitset(s);
    }
    void write(auto& io) const {
        _for (i, len(a) - (n & 63 ? 1 : 0))
            _for (j, 8)
                io.writestr((const char*)&O[a[i] >> (8 * j) & 255], 8);
        _for (j, n & 63)
            io.putch('0' + (a.back() >> j & 1));
    }
#endif
    str to_string() const {
        str s;
        s.reserve(len(a) << 6);
        _for (i, len(a))
            _for (j, 8)
                s.append((const char*)&O[a[i] >> (8 * j) & 255], 8);
        s.resize(n);
        return s;
    }
};