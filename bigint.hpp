#pragma once
#include "template.hpp"
#include "montgomery.hpp"

template <typename mint>
struct NTT {
    static constexpr u32 get_pr() {
        u64 ds[32] = {};
        int idx = 0;
        u64 m = mod - 1;
        for (u64 i = 2; i * i <= m; i++) {
            if (m % i == 0) {
                ds[idx++] = i;
                while (m % i == 0)
                    m /= i;
            }
        }
        if (m != 1)
            ds[idx++] = m;

        u32 _pr = 2;
        loop {
            int flg = 1;
            _for (i, idx) {
                u64 a = _pr, b = (mod - 1) / ds[i], r = 1;
                while (b) {
                    if (b & 1)
                        r = r * a % mod;
                    a = a * a % mod;
                    b >>= 1;
                }
                if (r == 1) {
                    flg = 0;
                    break;
                }
            }
            if (flg == 1)
                break;
            ++_pr;
        }
        return _pr;
    };

    static constexpr u32 mod = mint::mod();
    static constexpr mint pr = get_pr();
    static constexpr int lvl = __builtin_ctz(mod - 1);
    mint dw[lvl], dy[lvl];

    void setwy(int k) {
        mint w[lvl], y[lvl];
        w[k - 1] = power(pr, (mod - 1) / (1 << k));
        y[k - 1] = w[k - 1].inv();
        for (int i = k - 2; i > 0; --i)
            w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        for (int i = 3; i < k; i++) {
            dw[i] = dw[i - 1] * y[i - 2] * w[i];
            dy[i] = dy[i - 1] * w[i - 2] * y[i];
        }
    }

    NTT() { setwy(lvl); }

    void fft4(Vec<mint>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        if (k & 1) {
            int v = 1 << (k - 1);
            _for (j, v) {
                mint ajv = a[j + v];
                a[j + v] = a[j] - ajv;
                a[j] += ajv;
            }
        }
        int u = 1 << (2 + (k & 1));
        int v = 1 << (k - 2 - (k & 1));
        mint one = mint(1);
        mint imag = dw[1];
        while (v) {
            // jh = 0
            {
                int j0 = 0;
                int j1 = v;
                int j2 = j1 + v;
                int j3 = j2 + v;
                for (; j0 < v; j0++, j1++, j2++, j3++) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            // jh >= 1
            mint ww = one, xx = one * dw[2], wx = one;
            for (int jh = 4; jh < u;) {
                ww = xx * xx, wx = ww * xx;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for (; j0 < je; j0++, j2++) {
                    mint t0 = a[j0], t1 = a[j0 + v] * xx, t2 = a[j2] * ww,
                         t3 = a[j2 + v] * wx;
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j0 + v] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j2 + v] = t0m2 - t1m3;
                }
                xx *= dw[__builtin_ctz(jh += 4)];
            }
            u <<= 2;
            v >>= 2;
        }
    }

    void ifft4(Vec<mint>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        int u = 1 << (k - 2);
        int v = 1;
        mint one(1);
        mint imag = dy[1];
        while (u) {
            // jh = 0
            {
                int j0 = 0;
                int j1 = v;
                int j2 = v + v;
                int j3 = j2 + v;
                for (; j0 < v; ++j0, ++j1, ++j2, ++j3) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                    a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                    a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
                }
            }
            // jh >= 1
            mint ww = one, xx = one * dy[2], yy = one;
            u <<= 2;
            for (int jh = 4; jh < u;) {
                ww = xx * xx, yy = xx * imag;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for (; j0 < je; ++j0, ++j2) {
                    mint t0 = a[j0], t1 = a[j0 + v], t2 = a[j2], t3 = a[j2 + v];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j0 + v] = t0m1 + t2m3, a[j2 + v] = (t0m1 - t2m3) * ww;
                }
                xx *= dy[__builtin_ctz(jh += 4)];
            }
            u >>= 4;
            v <<= 2;
        }
        if (k & 1) {
            u = 1 << (k - 1);
            _for (j, u) {
                mint ajv = a[j] - a[j + u];
                a[j] += a[j + u];
                a[j + u] = ajv;
            }
        }
    }

    void ntt(Vec<mint>& a) {
        if (len(a) <= 1)
            return;
        fft4(a, __builtin_ctz(len(a)));
    }

    Vec<mint> multiply(const Vec<mint>& a, const Vec<mint>& b) {
        int l = len(a) + len(b) - 1;
        if (min(len(a), len(b)) <= 40) {
            Vec<mint> s(l);
            _for (i, len(a))
                _for (j, len(b))
                    s[i + j] += a[i] * b[j];
            return s;
        }
        int k = 2, M = 4;
        while (M < l)
            M <<= 1, k++;
        setwy(k);
        Vec<mint> s(M);
        _for (i, len(a))
            s[i] = a[i];
        fft4(s, k);
        if (len(a) == len(b) && a == b) {
            _for (i, M)
                s[i] *= s[i];
        }
        else {
            Vec<mint> t(M);
            _for (i, len(b))
                t[i] = b[i];
            fft4(t, k);
            _for (i, M)
                s[i] *= t[i];
        }
        ifft4(s, k);
        s.resize(l);
        mint invm = mint(M).inv();
        _for (i, l)
            s[i] *= invm;
        return s;
    }
};

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

    template <typename mint>
    Vec<mint> mul(const vi& a, const vi& b) {
        static NTT<mint> ntt;
        Vec<mint> s(len(a)), t(len(b));
        _for (i, len(a))
            s[i] = a[i] % mint::mod();
        _for (i, len(b))
            t[i] = b[i] % mint::mod();
        return ntt.multiply(s, t);
    }
    Vec<u128> multiply_u128(const vi& s, const vi& t) {
        if (s.empty() && t.empty())
            return {};
        if (min(len(s), len(t)) < 128) {
            Vec<u128> ret(len(s) + len(t) - 1);
            _for (i, len(s))
                _for (j, len(t))
                    ret[i + j] += u128(i64(s[i]) * t[j]);
            return ret;
        }
        auto d0 = mul<mint0>(s, t);
        auto d1 = mul<mint1>(s, t);
        auto d2 = mul<mint2>(s, t);
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

namespace bigintImpl {
    struct TENS {
        static constexpr int offset = 30;
        constexpr TENS() {
            t[offset] = 1;
            for (int i = 1; i <= offset; i++) {
                t[offset + i] = t[offset + i - 1] * 10.0;
                t[offset - i] = 1.0 / t[offset + i];
            }
        }
        [[nodiscard]] ld ten_ld(int n) const { return t[n + offset]; }

    private:
        ld t[offset * 2 + 1]{};
    };
    // 0: neg = false, dat = {}
    struct bigint {
        using M = bigint;
        inline static constexpr bigintImpl::TENS tens{};

        static constexpr int logD = 9;
        static constexpr int D = ten(logD);
        bool neg = false;
        vi dat;

        bigint() = default;
        bigint(bool n, vi d): neg(n), dat(std::move(d)) {}

        bigint(Integer auto x) {
            if constexpr (Signed<decltype(x)>)
                if (x < 0)
                    neg = true, x = -x;
            while (x)
                dat.push_back(x % D), x /= D;
        }

        bigint(std::string_view s) {
            if (len(s) == 1 && s[0] == '0')
                return;
            int l = 0;
            if (s[0] == '-')
                l++, neg = true;
            for (int ie = len(s); l < ie; ie -= logD) {
                int is = max(l, ie - logD);
                i64 x = 0;
                _for (i, is, ie)
                    x = x * 10 + s[i] - '0';
                dat.push_back(x);
            }
        }
#ifdef FASTIO
        void read(IO& io) {
            static str s;
            io.read(s);
            *this = s;
        }
        void write(IO& io) {
            if (is_zero()) {
                io.putch('0');
                return;
            }
            if (neg)
                io.putch('-');
            _for_r (i, _size())
                io.write(_itos(dat[i], i != _size() - 1));
        }
#endif
        friend M operator+(const M& lhs, const M& rhs) {
            if (lhs.neg == rhs.neg)
                return {lhs.neg, _add(lhs.dat, rhs.dat)};
            if (_leq(lhs.dat, rhs.dat)) {
                // |l| <= |r|
                auto c = _sub(rhs.dat, lhs.dat);
                bool n = _is_zero(c) ? false : rhs.neg;
                return {n, c};
            }
            auto c = _sub(lhs.dat, rhs.dat);
            bool n = _is_zero(c) ? false : lhs.neg;
            return {n, c};
        }
        friend M operator-(const M& lhs, const M& rhs) { return lhs + (-rhs); }

        friend M operator*(const M& lhs, const M& rhs) {
            auto c = _mul(lhs.dat, rhs.dat);
            bool n = _is_zero(c) ? false : (lhs.neg ^ rhs.neg);
            return {n, c};
        }
        friend auto divmod(const M& lhs, const M& rhs) {
            auto dm = _divmod_newton(lhs.dat, rhs.dat);
            bool dn = _is_zero(dm.first) ? false : lhs.neg != rhs.neg;
            bool mn = _is_zero(dm.second) ? false : lhs.neg;
            return std::pair{M{dn, dm.first}, M{mn, dm.second}};
        }
        friend M operator/(const M& lhs, const M& rhs) {
            return divmod(lhs, rhs).first;
        }
        friend M operator%(const M& lhs, const M& rhs) {
            return divmod(lhs, rhs).second;
        }

        M& operator+=(const M& rhs) { return *this = *this + rhs; }
        M& operator-=(const M& rhs) { return *this = *this - rhs; }
        M& operator*=(const M& rhs) { return *this = *this * rhs; }
        M& operator/=(const M& rhs) { return *this = *this / rhs; }
        M& operator%=(const M& rhs) { return *this = *this % rhs; }

        M operator-() const {
            if (is_zero())
                return *this;
            return {!neg, dat};
        }
        M operator+() const { return *this; }
        friend M abs(const M& m) { return {false, m.dat}; }
        [[nodiscard]] bool is_zero() const { return _is_zero(dat); }

        friend bool operator==(const M& lhs, const M& rhs) {
            return lhs.neg == rhs.neg && lhs.dat == rhs.dat;
        }
        friend bool operator!=(const M& lhs, const M& rhs) {
            return lhs.neg != rhs.neg || lhs.dat != rhs.dat;
        }
        friend bool operator<(const M& lhs, const M& rhs) {
            if (lhs == rhs)
                return false;
            return _neq_lt(lhs, rhs);
        }
        friend bool operator<=(const M& lhs, const M& rhs) {
            if (lhs == rhs)
                return true;
            return _neq_lt(lhs, rhs);
        }
        friend bool operator>(const M& lhs, const M& rhs) {
            if (lhs == rhs)
                return false;
            return _neq_lt(rhs, lhs);
        }
        friend bool operator>=(const M& lhs, const M& rhs) {
            if (lhs == rhs)
                return true;
            return _neq_lt(rhs, lhs);
        }

        // a * 10^b (1 <= |a| < 10)
        [[nodiscard]] std::pair<ld, int> dfp() const {
            if (is_zero())
                return {};
            int l = max(0, _size() - 3);
            int b = logD * l;
            str prefix;
            for (int i = _size() - 1; i >= l; i--)
                prefix += _itos(dat[i], i != _size() - 1);
            b += len(prefix) - 1;
            ld a = 0;
            foreach (c, prefix)
                a = a * 10.0 + (c - '0');
            a *= tens.ten_ld(-len(prefix) + 1);
            a = std::clamp<ld>(a, 1.0, nextafterl(10.0, 1.0));
            if (neg)
                a = -a;
            return {a, b};
        }
        [[nodiscard]] str to_string() const {
            if (is_zero())
                return "0";
            str r;
            if (neg)
                r.push_back('-');
            _for_r (i, _size())
                r += _itos(dat[i], i != _size() - 1);
            return r;
        }
        [[nodiscard]] ld to_ld() const {
            auto [a, b] = dfp();
            if (-bigintImpl::TENS::offset <= b && b <= bigintImpl::TENS::offset)
                return a * tens.ten_ld(b);
            return a * powl(10, b);
        }
        [[nodiscard]] i64 to_ll() const {
            i64 r = _to_ll(dat);
            return neg ? -r : r;
        }
        [[nodiscard]] i128 to_i128() const {
            i128 r = _to_i128(dat);
            return neg ? -r : r;
        }

    private:
        [[nodiscard]] int _size() const { return len(dat); }
        static bool _lt(const vi& a, const vi& b) {
            if (len(a) != len(b))
                return len(a) < len(b);
            _for_r (i, len(a))
                if (a[i] != b[i])
                    return a[i] < b[i];
            return false;
        }
        static bool _leq(const vi& a, const vi& b) {
            return a == b || _lt(a, b);
        }
        static bool _neq_lt(const M& lhs, const M& rhs) {
            if (lhs.neg != rhs.neg)
                return lhs.neg;
            return _lt(lhs.dat, rhs.dat) ^ lhs.neg;
        }
        static bool _is_zero(const vi& a) { return a.empty(); }
        static bool _is_one(const vi& a) { return len(a) == 1 && a[0] == 1; }
        static void _shrink(vi& a) {
            while (len(a) && a.back() == 0)
                a.pop_back();
        }
        static vi _add(const vi& a, const vi& b) {
            vi c(max(len(a), len(b)) + 1);
            _for (i, len(a))
                c[i] += a[i];
            _for (i, len(b))
                c[i] += b[i];
            _for (i, len(c) - 1)
                if (c[i] >= D)
                    c[i] -= D, c[i + 1]++;
            _shrink(c);
            return c;
        }
        static vi _sub(const vi& a, const vi& b) {
            vi c{a};
            int t = 0;
            _for (i, len(a)) {
                if (i < len(b))
                    t += b[i];
                c[i] -= t;
                t = 0;
                if (c[i] < 0)
                    c[i] += D, t = 1;
            }
            _shrink(c);
            return c;
        }
        static vi _mul_fft(const vi& a, const vi& b) {
            if (a.empty() || b.empty())
                return {};
            auto m = ArbitraryNTT::multiply_u128(a, b);
            vi c;
            c.reserve(len(m) + 3);
            u128 x = 0;
            for (int i = 0;; i++) {
                if (i >= len(m) && x == 0)
                    break;
                if (i < len(m))
                    x += m[i];
                c.push_back(x % D);
                x /= D;
            }
            _shrink(c);
            return c;
        }
        static vi _mul_naive(const vi& a, const vi& b) {
            if (a.empty() || b.empty())
                return {};
            Vec<i64> prod(len(a) + len(b));
            _for (i, len(a)) {
                _for (j, len(b)) {
                    i64 p = i64(a[i]) * b[j];
                    prod[i + j] += p;
                    if (prod[i + j] >= 4LL * D * D) {
                        prod[i + j] -= 4LL * D * D;
                        prod[i + j + 1] += 4LL * D;
                    }
                }
            }
            vi c(len(prod) + 1);
            i64 x = 0;
            int i = 0;
            for (; i < len(prod); i++)
                x += prod[i], c[i] = x % D, x /= D;
            while (x)
                c[i] = x % D, x /= D, i++;
            _shrink(c);
            return c;
        }
        static vi _mul(const vi& a, const vi& b) {
            if (_is_zero(a) || _is_zero(b))
                return {};
            if (_is_one(a))
                return b;
            if (_is_one(b))
                return a;
            if (min(len(a), len(b)) <= 128)
                return len(a) < len(b) ? _mul_naive(b, a) : _mul_naive(a, b);
            return _mul_fft(a, b);
        }
        static auto _divmod_li(const vi& a, const vi& b) {
            i64 va = _to_ll(a);
            int vb = b[0];
            return std::pair{_integer_to_vec(va / vb), _integer_to_vec(va % vb)};
        }
        static auto _divmod_ll(const vi& a, const vi& b) {
            i64 va = _to_ll(a), vb = _to_ll(b);
            return std::pair{_integer_to_vec(va / vb), _integer_to_vec(va % vb)};
        }
        static auto _divmod_1e9(const vi& a, const vi& b) {
            if (b[0] == 1)
                return std::pair{a, vi{}};
            if (len(a) <= 2)
                return _divmod_li(a, b);
            vi quo(len(a));
            i64 d = 0;
            int b0 = b[0];
            _for_r (i, len(a)) {
                d = d * D + a[i];
                int q = d / b0, r = d % b0;
                quo[i] = q, d = r;
            }
            _shrink(quo);
            return std::pair{quo, d ? vi{int(d)} : vi{}};
        }
        static auto _divmod_naive(const vi& a, const vi& b) {
            assert(!_is_zero(b));
            if (len(b) == 1)
                return _divmod_1e9(a, b);
            if (max(len(a), len(b)) <= 2)
                return _divmod_ll(a, b);
            if (_lt(a, b))
                return std::pair{vi{}, a};
            int norm = D / (b.back() + 1);
            vi x = _mul(a, {norm});
            vi y = _mul(b, {norm});
            int yb = y.back();
            vi quo(len(x) - len(y) + 1);
            vi rem(x.end() - len(y), x.end());
            _for_r (i, len(quo)) {
                if (len(rem) == len(y)) {
                    if (_leq(y, rem))
                        quo[i] = 1, rem = _sub(rem, y);
                }
                else if (len(rem) > len(y)) {
                    i64 rb = i64(rem[len(rem) - 1]) * D + rem[len(rem) - 2];
                    int q = rb / yb;
                    vi yq = _mul(y, {q});
                    while (_lt(rem, yq))
                        q--, yq = _sub(yq, y);
                    rem = _sub(rem, yq);
                    while (_leq(y, rem))
                        q++, rem = _sub(rem, y);
                    quo[i] = q;
                }
                if (i)
                    rem.insert(rem.begin(), x[i - 1]);
            }
            _shrink(quo), _shrink(rem);
            auto [q2, r2] = _divmod_1e9(rem, {norm});
            return std::pair{quo, q2};
        }
        static vi _calc_inv(const vi& a, int deg) {
            int k = deg, c = len(a);
            while (k > 64)
                k = (k + 1) / 2;
            vi z(c + k + 1);
            z.back() = 1;
            z = _divmod_naive(z, a).first;
            while (k < deg) {
                vi s = _mul(z, z);
                s.insert(s.begin(), 0);
                int d = min(c, 2 * k + 1);
                vi t(a.end() - d, a.end()), u = _mul(s, t);
                u.erase(u.begin(), u.begin() + d);
                vi w(k + 1), w2 = _add(z, z);
                std::copy(all(w2), std::back_inserter(w));
                z = _sub(w, u);
                z.erase(z.begin());
                k *= 2;
            }
            z.erase(z.begin(), z.begin() + k - deg);
            return z;
        }
        static std::pair<vi, vi> _divmod_newton(const vi& a, const vi& b) {
            assert(!_is_zero(b));
            if (len(b) <= 64 || len(a) - len(b) <= 64)
                return _divmod_naive(a, b);
            int norm = D / (b.back() + 1);
            vi x = _mul(a, {norm});
            vi y = _mul(b, {norm});
            int s = len(x), t = len(y);
            int deg = s - t + 2;
            vi z = _calc_inv(y, deg);
            vi q = _mul(x, z);
            q.erase(q.begin(), q.begin() + t + deg);
            vi yq = _mul(y, {q});
            while (_lt(x, yq))
                q = _sub(q, {1}), yq = _sub(yq, y);
            vi r = _sub(x, yq);
            while (_leq(y, r))
                q = _add(q, {1}), r = _sub(r, y);
            _shrink(q), _shrink(r);
            auto [q2, r2] = _divmod_1e9(r, {norm});
            return {q, q2};
        }
        static str _itos(int x, bool zero_padding) {
            str r;
            _for (i, logD)
                r.push_back('0' + x % 10), x /= 10;
            if (!zero_padding)
                while (!r.empty() && r.back() == '0')
                    r.pop_back();
            reverse(r);
            return r;
        }
        static vi _integer_to_vec(auto x) {
            vi r;
            while (x)
                r.push_back(x % D), x /= D;
            return r;
        }
        static i64 _to_ll(const vi& a) {
            i64 r = 0;
            _for_r (i, len(a))
                r = r * D + a[i];
            return r;
        }
        static i128 _to_i128(const vi& a) {
            i128 r = 0;
            _for_r (i, len(a))
                r = r * D + a[i];
            return r;
        }
    };
} // namespace bigintImpl
using bigint = bigintImpl::bigint;