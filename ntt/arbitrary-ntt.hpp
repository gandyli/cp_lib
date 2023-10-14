#pragma once
#include "ntt-avx2.hpp"

namespace ArbitraryNTT {
    constexpr int m0 = 167772161;
    constexpr int m1 = 469762049;
    constexpr int m2 = 754974721;
    using mint0 = MMInt<m0>;
    using mint1 = MMInt<m1>;
    using mint2 = MMInt<m2>;
    constexpr int r01 = mint1(m0).inv().val();
    constexpr int r02 = mint2(m0).inv().val();
    constexpr int r12 = mint2(m1).inv().val();
    constexpr int r02r12 = i64(r02) * r12 % m2;
    constexpr i64 w1 = m0;
    constexpr i64 w2 = i64(m0) * m1;

    template <typename T, typename mint>
    Vec<mint> mul(const Vec<T>& a, const Vec<T>& b) {
        static NTT<mint> ntt;
        Vec<mint> s(len(a)), t(len(b));
        _for (i, len(a))
            s[i] = a[i] % mint::mod();
        _for (i, len(b))
            t[i] = b[i] % mint::mod();
        return ntt.multiply(s, t);
    }
    template <typename T>
    vi multiply(const Vec<T>& s, const Vec<T>& t, int mod) {
        auto d0 = mul<T, mint0>(s, t);
        auto d1 = mul<T, mint1>(s, t);
        auto d2 = mul<T, mint2>(s, t);
        int n = len(d0);
        vi ret(n);
        const int W1 = w1 % mod;
        const int W2 = w2 % mod;
        _for (i, n) {
            int n1 = d1[i].val(), n2 = d2[i].val(), a = d0[i].val();
            int b = i64(n1 + m1 - a) * r01 % m1;
            int c = (i64(n2 + m2 - a) * r02r12 + i64(m2 - b) * r12) % m2;
            ret[i] = (i64(a) + i64(b) * W1 + i64(c) * W2) % mod;
        }
        return ret;
    }

    template <typename mint>
    Vec<mint> multiply(const Vec<mint>& a, const Vec<mint>& b) {
        if (a.empty() && b.empty())
            return {};
        if (min(len(a), len(b)) < 128) {
            Vec<mint> ret(len(a) + len(b) - 1);
            _for (i, len(a))
                _for (j, len(b))
                    ret[i + j] += a[i] * b[j];
            return ret;
        }
        vi s(len(a)), t(len(b));
        _for (i, len(a))
            s[i] = a[i].val();
        _for (i, len(b))
            t[i] = b[i].val();
        vi u = multiply<int>(s, t, mint::mod());
        return {all(u)};
    }
    template <typename T>
    Vec<u128> multiply_u128(const Vec<T>& s, const Vec<T>& t) {
        if (s.empty() && t.empty())
            return {};
        if (min(len(s), len(t)) < 128) {
            Vec<u128> ret(len(s) + len(t) - 1);
            _for (i, len(s))
                _for (j, len(t))
                    ret[i + j] += u128(i64(s[i]) * t[j]);
            return ret;
        }
        auto d0 = mul<T, mint0>(s, t);
        auto d1 = mul<T, mint1>(s, t);
        auto d2 = mul<T, mint2>(s, t);
        int n = len(d0);
        Vec<u128> ret(n);
        _for (i, n) {
            i64 n1 = d1[i].val(), n2 = d2[i].val();
            i64 a = d0[i].val();
            i64 b = (n1 + m1 - a) * r01 % m1;
            i64 c = ((n2 + m2 - a) * r02r12 + (m2 - b) * r12) % m2;
            ret[i] = a + b * w1 + u128(c) * w2;
        }
        return ret;
    }
} // namespace ArbitraryNTT
