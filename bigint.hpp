#pragma once
#include "arbitrary-ntt.hpp"

namespace impl {
    struct TENS {
        static constexpr int offset = 30;
        constexpr TENS() {
            t[offset] = 1;
            _for (i, 1, offset + 1) {
                t[offset + i] = t[offset + i - 1] * 10.0;
                t[offset - i] = 1.0 / t[offset + i];
            }
        }
        [[nodiscard]] ld ten_ld(int n) const { return t[n + offset]; }

    private:
        ld t[offset << 1 | 1]{};
    };
} // namespace impl
// 0: neg = false, dat = {}
struct bigint {
private:
    inline static constexpr impl::TENS tens{};

public:
    using M = bigint;

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
        dat = _from_int(x);
    }

    bigint(std::string_view s) {
        if (len(s) == 1 && s[0] == '0')
            return;
        int l = 0;
        if (s[0] == '-')
            l++, neg = true;
        for (int ie = len(s); l < ie; ie -= logD) {
            int is = max(l, ie - logD);
            int x = 0;
            _for (i, is, ie)
                x = x * 10 + s[i] - '0';
            dat.eb(x);
        }
    }
#ifdef FASTIO
    void read(IO& io) {
        static str s;
        io.read(s);
        *this = bigint(s);
    }
    void write(IO& io) const {
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
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = rhs.neg && !_is_zero(c);
            return {n, c};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !_is_zero(c);
        return {n, std::move(c)};
    }
    friend M operator-(const M& lhs, const M& rhs) {
        if (rhs.is_zero())
            return lhs;
        if (lhs.neg == !rhs.neg)
            return {lhs.neg, _add(lhs.dat, rhs.dat)};
        if (_leq(lhs.dat, rhs.dat)) {
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = !rhs.neg && !_is_zero(c);
            return {n, c};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !_is_zero(c);
        return {n, std::move(c)};
    }

    friend M operator*(const M& lhs, const M& rhs) {
        auto c = _mul(lhs.dat, rhs.dat);
        bool n = (lhs.neg ^ rhs.neg) && !_is_zero(c);
        return {n, std::move(c)};
    }
    friend auto divmod(const M& lhs, const M& rhs) {
        auto dm = _divmod_newton(lhs.dat, rhs.dat);
        bool dn = lhs.neg != rhs.neg && !_is_zero(dm.first);
        bool mn = lhs.neg && !_is_zero(dm.second);
        return std::pair{M{dn, std::move(dm.first)}, M{mn, std::move(dm.second)}};
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

    friend bool operator==(const M& lhs, const M& rhs) = default;
    friend bool operator<(const M& lhs, const M& rhs) { return lhs != rhs && _neq_lt(lhs, rhs); }
    friend bool operator<=(const M& lhs, const M& rhs) { return !(rhs < lhs); }
    friend bool operator>(const M& lhs, const M& rhs) { return rhs < lhs; }
    friend bool operator>=(const M& lhs, const M& rhs) { return !(lhs < rhs); }

    [[nodiscard]] std::pair<ld, int> dfp() const {
        if (is_zero())
            return {};
        int l = max(0, _size() - 3);
        int b = logD * l;
        str prefix;
        _for_r (i, l, _size())
            prefix += _itos(dat[i], i != _size() - 1);
        b += len(prefix) - 1;
        ld a = 0;
        foreach (c, prefix)
            a = a * 10.L + (c - '0');
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
            r += '-';
        _for_r (i, _size())
            r += _itos(dat[i], i != _size() - 1);
        return r;
    }
    [[nodiscard]] ld to_ld() const {
        auto [a, b] = dfp();
        if (-impl::TENS::offset <= b && b <= impl::TENS::offset)
            return a * tens.ten_ld(b);
        return a * powl(10, b);
    }
    template <typename T>
    [[nodiscard]] T to_int() const {
        T r = _to_int<T>(dat);
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
        while (!a.empty() && !a.back())
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
            c.eb(x % D);
            x /= D;
        }
        _shrink(c);
        return c;
    }
    static vi _mul_naive(const vi& a, const vi& b) {
        if (a.empty() || b.empty())
            return {};
        Vec<i64> prod(len(a) + len(b));
        _for (i, len(a))
            _for (j, len(b))
                if (prod[i + j] += i64(a[i]) * b[j]; prod[i + j] >= 4LL * D * D) {
                    prod[i + j] -= 4LL * D * D;
                    prod[i + j + 1] += 4LL * D;
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
        i64 va = _to_int<i64>(a);
        int vb = b[0];
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
    }
    static auto _divmod_i64(const vi& a, const vi& b) {
        i64 va = _to_int<i64>(a), vb = _to_int<i64>(b);
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
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
        return std::pair{std::move(quo), d ? vi{int(d)} : vi{}};
    }
    static auto _divmod_naive(const vi& a, const vi& b) {
        assert(!_is_zero(b));
        if (len(b) == 1)
            return _divmod_1e9(a, b);
        if (max(len(a), len(b)) <= 2)
            return _divmod_i64(a, b);
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
        return std::pair{std::move(quo), std::move(q2)};
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
            copy(w2, back_inserter(w));
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
        return {std::move(q), std::move(q2)};
    }
    static str _itos(int x, bool zero_padding) {
        str r;
        _for (i, logD)
            r += '0' + x % 10, x /= 10;
        if (!zero_padding)
            while (!r.empty() && r.back() == '0')
                r.pop_back();
        reverse(r);
        return r;
    }
    static vi _from_int(Integer auto x) {
        vi r;
        while (x)
            r.eb(x % D), x /= D;
        return r;
    }
    template <typename T>
    static T _to_int(const vi& a) {
        T r{};
        _for_r (i, len(a))
            r = r * D + a[i];
        return r;
    }
};
