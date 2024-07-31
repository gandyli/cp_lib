#pragma once
#include "poly/fft.hpp"
#include "poly/convolution_naive.hpp"
#include "utility/itos_table.hpp"

// 0: neg = false, dat = {}
struct bigint {
    using M = bigint;

    static constexpr int logD = 8;
    static constexpr u32 D = ten(logD), B = ten(4);
    bool neg = false;
    vi dat;

    bigint() = default;
    bigint(bool n, vi d): neg(n), dat(std::move(d)) {}

    template <Signed T>
    bigint(T x) {
        make_unsigned_t<T> y = x;
        if (x < 0)
            neg = true, y = -y;
        dat = _from_int(y);
    }
    template <Unsigned T>
    bigint(T x): dat(_from_int(x)) {}

    bigint(std::string_view s) {
        if (s == "0")
            return;
        if (s[0] == '-')
            s = s.substr(1), neg = true;
        int q = ceil(len(s), logD), r = len(s) % logD;
        if (r == 0)
            r = logD;
        dat.resize(q);
        std::from_chars(s.data(), s.data() + r, dat.back());
        auto load8 = [&](auto s) {
            u64 x = *(u64*)s ^ 0x3030303030303030;
            x = ((x * 2561) >> 8) & 0xff00ff00ff00ff;
            x = ((x * 6553601) >> 16) & 0xffff0000ffff;
            return (x * 42949672960001) >> 32;
        };
        _for_r (i, q - 1)
            dat[i] = load8(s.data() + r), r += logD;
        _shrink(dat);
    }
#ifdef FASTIO
    void read(auto& io) {
        static str s;
        io.read(s);
        *this = bigint(s);
    }
    void write(auto& io) const {
        if (dat.empty()) {
            io.putch('0');
            return;
        }
        if (neg)
            io.putch('-');
        io.write(dat.back());
        _for_r (i, len(dat) - 1) {
            io.writestr(&itos_table[dat[i] / B * 4], logD - 4);
            io.writestr(&itos_table[dat[i] % B * 4], 4);
        }
    }
#endif
    friend M operator+(const M& lhs, const M& rhs) {
        if (lhs.neg == rhs.neg)
            return {lhs.neg, _add(lhs.dat, rhs.dat)};
        if (_leq(lhs.dat, rhs.dat)) {
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = rhs.neg && !c.empty();
            return {n, std::move(c)};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !c.empty();
        return {n, std::move(c)};
    }
    friend M operator-(const M& lhs, const M& rhs) {
        if (rhs.dat.empty())
            return lhs;
        if (lhs.neg == !rhs.neg)
            return {lhs.neg, _add(lhs.dat, rhs.dat)};
        if (_leq(lhs.dat, rhs.dat)) {
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = !rhs.neg && !c.empty();
            return {n, std::move(c)};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !c.empty();
        return {n, std::move(c)};
    }

    friend M operator*(const M& lhs, const M& rhs) {
        auto c = _mul(lhs.dat, rhs.dat);
        bool n = (lhs.neg ^ rhs.neg) && !c.empty();
        return {n, std::move(c)};
    }
    friend auto divmod(const M& lhs, const M& rhs) {
        auto dm = _divmod_newton(lhs.dat, rhs.dat);
        bool dn = lhs.neg != rhs.neg && !dm.first.empty();
        bool mn = lhs.neg && !dm.second.empty();
        return std::pair{M{dn, std::move(dm.first)}, M{mn, std::move(dm.second)}};
    }
    friend M operator/(const M& lhs, const M& rhs) { return divmod(lhs, rhs).first; }
    friend M operator%(const M& lhs, const M& rhs) { return divmod(lhs, rhs).second; }

    M& operator+=(const M& rhs) { return *this = *this + rhs; }
    M& operator-=(const M& rhs) { return *this = *this - rhs; }
    M& operator*=(const M& rhs) { return *this = *this * rhs; }
    M& operator/=(const M& rhs) { return *this = *this / rhs; }
    M& operator%=(const M& rhs) { return *this = *this % rhs; }

    M& operator++() { return *this += 1; }
    M operator++(int) {
        M t = *this;
        ++*this;
        return t;
    }
    M& operator--() { return *this -= 1; }
    M operator--(int) {
        M t = *this;
        --*this;
        return t;
    }

    M operator-() const {
        if (dat.empty())
            return *this;
        return {!neg, dat};
    }
    M operator+() const { return *this; }
    friend M abs(const M& m) { return {false, m.dat}; }

    friend bool operator==(const M& lhs, const M& rhs) = default;
    friend bool operator<(const M& lhs, const M& rhs) { return lhs != rhs && _neq_lt(lhs, rhs); }
    friend bool operator<=(const M& lhs, const M& rhs) { return !(rhs < lhs); }
    friend bool operator>(const M& lhs, const M& rhs) { return rhs < lhs; }
    friend bool operator>=(const M& lhs, const M& rhs) { return !(lhs < rhs); }

    std::pair<ld, int> dfp() const {
        if (dat.empty())
            return {};
        int l = max(0, len(dat) - 3);
        int b = logD * l;
        str prefix;
        _for_r (i, l, len(dat))
            prefix += _itos(dat[i], i != len(dat) - 1);
        b += len(prefix) - 1;
        ld a = 0;
        foreach (c, prefix)
            a = a * 10 + (c - '0');
        a *= powl(10, -len(prefix) + 1);
        a = std::clamp<ld>(a, 1.0, nextafterl(10.0, 1.0));
        if (neg)
            a = -a;
        return {a, b};
    }
    str to_string() const {
        if (dat.empty())
            return "0";
        str r;
        r.reserve(len(dat) * logD + 1);
        if (neg)
            r += '-';
        r += _itos(dat.back(), false);
        _for_r (i, len(dat) - 1) {
            r.append(&itos_table[dat[i] / B * 4], logD - 4);
            r.append(&itos_table[dat[i] % B * 4], 4);
        }
        return r;
    }
    ld to_ld() const {
        auto [a, b] = dfp();
        return a * powl(10, b);
    }
    template <typename T>
    T to_int() const {
        T r = _to_int<T>(dat);
        return neg ? -r : r;
    }

private:
    static bool _lt(const vi& a, const vi& b) {
        if (len(a) != len(b))
            return len(a) < len(b);
        _for_r (i, len(a))
            if (a[i] != b[i])
                return a[i] < b[i];
        return false;
    }
    static bool _leq(const vi& a, const vi& b) { return a == b || _lt(a, b); }
    static bool _neq_lt(const M& lhs, const M& rhs) {
        if (lhs.neg != rhs.neg)
            return lhs.neg;
        return _lt(lhs.dat, rhs.dat) ^ lhs.neg;
    }
    static bool _is_one(const vi& a) { return len(a) == 1 && a[0] == 1; }
    static void _shrink(vi& a) {
        while (!a.empty() && !a.back())
            a.pop_back();
    }
    static vi _add(const vi& a, const vi& b) {
        vi c(max(len(a), len(b)) + 1);
        _for (i, max(len(a), len(b))) {
            if (i < len(a))
                c[i] += a[i];
            if (i < len(b))
                c[i] += b[i];
            if (c[i] >= D)
                c[i] -= D, c[i + 1]++;
        }
        _shrink(c);
        return c;
    }
    static vi _sub(vi a, const vi& b) {
        int t = 0;
        _for (i, len(a)) {
            if (i < len(b))
                t += b[i];
            a[i] -= t;
            t = 0;
            if (a[i] < 0)
                a[i] += D, t = 1;
        }
        _shrink(a);
        return a;
    }
    static vi _mul_naive(const vi& a, const vi& b) {
        auto c = convolution_naive<int, u64>(a, b);
        vi r(len(a) + len(b) + 2);
        u64 x = 0;
        _for (i, len(c)) {
            x += c[i];
            r[i] = x % D, x /= D;
        }
        int i = len(c);
        while (x)
            r[i++] = x % D, x /= D;
        _shrink(r);
        return r;
    }
    static vi _mul(const vi& a, const vi& b) {
        using namespace CFFT;
        if (a.empty() || b.empty())
            return {};
        if (_is_one(a))
            return b;
        if (_is_one(b))
            return a;
        int n = len(a), m = len(b);
        if (min(n, m) <= 30)
            return _mul_naive(a, b);
        int k = get_lg(n + m), sz = 1 << k;
        vc<C> c(sz), d(sz);
        _for (i, n)
            c[i] = {f64(a[i] % B), f64(a[i] / B)};
        _for (i, m)
            d[i] = {f64(b[i] % B), f64(b[i] / B)};
        setw(k);
        fft(c, k), fft(d, k);
        f64 fx = 1.0 / sz, fy = fx * 0.25;
        c[0] = {c[0].x * d[0].x + c[0].y * d[0].y, c[0].x * d[0].y + c[0].y * d[0].x};
        c[0] *= fx;
        c[1] *= d[1] * fx;
        for (int k = 2, m = 3; k < sz; k <<= 1, m <<= 1)
            for (int i = k, j = i + k - 1; i < m; i++, j--) {
                C oi = c[i] + c[j].conj(), hi = c[i] - c[j].conj();
                C oj = d[i] + d[j].conj(), hj = d[i] - d[j].conj();
                C r0 = oi * oj - hi * hj * ((i & 1) ? -w[i >> 1] : w[i >> 1]);
                C r1 = oj * hi + oi * hj;
                c[i] = (r0 + r1) * fy;
                c[j] = (r0 - r1).conj() * fy;
            }
        ifft(c, k);
        vi r(n + m + 2);
        u64 x = 0;
        _for (i, n + m) {
            x += i64(c[i].x + 0.5) + i64(c[i].y + 0.5) * B;
            r[i] = x % D, x /= D;
        }
        int i = n + m;
        while (x)
            r[i++] = x % D, x /= D;
        _shrink(r);
        return r;
    }
    static auto _divmod_li(const vi& a, const vi& b) {
        i64 va = _to_int<i64>(a), vb = b[0];
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
    }
    static auto _divmod_i64(const vi& a, const vi& b) {
        i64 va = _to_int<i64>(a), vb = _to_int<i64>(b);
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
    }
    static auto _divmod_1e8(const vi& a, const vi& b) {
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
        ASSERT(!b.empty());
        if (len(b) == 1)
            return _divmod_1e8(a, b);
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
        auto [q2, r2] = _divmod_1e8(rem, {norm});
        return std::pair{std::move(quo), std::move(q2)};
    }
    static vi _calc_inv(const vi& a, int deg) {
        int k = deg, c = len(a);
        while (k > 64)
            k = (k + 1) >> 1;
        vi z(c + k + 1);
        z.back() = 1;
        z = _divmod_naive(z, a).first;
        while (k < deg) {
            vi s = _mul(z, z);
            s.insert(s.begin(), 0);
            int d = min(c, 2 * k + 1);
            vi t(a.end() - d, a.end()), u = _mul(s, t);
            u.erase(u.begin(), u.begin() + d);
            vi w(k + 1);
            concat(w, _add(z, z));
            z = _sub(w, u);
            z.erase(z.begin());
            k <<= 1;
        }
        z.erase(z.begin(), z.begin() + k - deg);
        return z;
    }
    static std::pair<vi, vi> _divmod_newton(const vi& a, const vi& b) {
        ASSERT(!b.empty());
        if (len(b) <= 64 || len(a) - len(b) <= 64)
            return _divmod_naive(a, b);
        int norm = D / (b.back() + 1);
        vi x = _mul(a, {norm}), y = _mul(b, {norm});
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
        auto [q2, r2] = _divmod_1e8(r, {norm});
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
