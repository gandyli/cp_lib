#pragma once
#include "modint/montgomery.hpp"
#include "random/base.hpp"

namespace impl {
    template <typename T>
    struct FactorizationResult {
        Vec<T> f, pf;
    };
    template <typename mint, typename T>
    T miller_rabin(mint a, T d, int r) {
        const mint one(1), mone(-1);
        mint x = power(a, d);
        if (x == one || x == mone)
            return 1;
        while (r--) {
            mint y = x * x;
            if (y == one)
                return std::gcd(x.val() - 1, mint::mod());
            x = y;
            if (x == mone)
                return 1;
        }
        return 0;
    }
    u32 prime_or_factor_32(u32 n) {
        if (n < 64)
            return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0)
            return 2;
        u32 g = std::gcd(n, 0xc0cfd797);
        if (g != 1)
            return g != n ? g : 0;
        SetMod(u32, n);
        int r = std::countr_zero(n - 1);
        u32 d = (n - 1) >> r;
        for (u32 a: {2, 7, 61}) {
            u32 ret = miller_rabin(mint(a), d, r);
            if (ret != 1)
                return ret;
        }
        return 1;
    }
    u64 prime_or_factor_64(u64 n) {
        if (n < 64)
            return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0)
            return 2;
        u64 g = std::gcd(n, 0xe221f97c30e94e1d);
        if (g != 1)
            return g != n ? g : 0;
        SetMod(u64, n);
        int r = std::countr_zero(n - 1);
        u64 d = (n - 1) >> r;
        for (u64 a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            u64 ret = miller_rabin(mint(a), d, r);
            if (ret != 1)
                return ret;
        }
        return 1;
    }
} // namespace impl
u64 prime_or_factor(Unsigned auto n) {
    if (n < (1 << 30))
        return impl::prime_or_factor_32(n);
    return impl::prime_or_factor_64(n);
}
namespace impl {
    int jacobi(i64 n, i64 k) {
        assert(k > 0 && k % 2 == 1);
        n %= k;
        int t = 1;
        while (n != 0) {
            while (n % 2 == 0) {
                n /= 2;
                int r = k % 8;
                if (r == 3 || r == 5)
                    t = -t;
            }
            std::swap(n, k);
            if (n % 4 == 3 && k % 4 == 3)
                t = -t;
            n %= k;
        }
        return k == 1 ? t : 0;
    }
    template <typename mint, typename T = typename mint::int_type>
    struct MontgomeryCurve {
        struct Point {
            mint x, z;
            T check() const { return std::gcd(z.val(), mint::mod()); }
        };
        MontgomeryCurve(mint a): _a24((a + 2) / 4) {}
        static std::pair<MontgomeryCurve, Point> random_curve_and_point() {
            loop {
                mint a(rnd()),
                  x(rnd()),
                  m1(1),
                  y2 = x * (x * (x + a) + m1);
                if (jacobi(y2.val(), mint::mod()) == 1)
                    return {MontgomeryCurve(a), Point{x, m1}};
            }
        }
        Point dbl(const Point& p) const {
            mint s = p.x + p.z,
                 d = p.x - p.z,
                 s2 = s * s,
                 d2 = d * d,
                 t = s2 - d2,
                 x = s2 * d2,
                 z = t * (d2 + _a24 * t);
            return {x, z};
        }
        Point add(const Point& p, const Point& q, const Point& diff) const {
            mint u = (p.x - p.z) * (q.x + q.z),
                 v = (p.x + p.z) * (q.x - q.z),
                 u_plus_v = u + v,
                 u_minus_v = u - v,
                 x = diff.z * u_plus_v * u_plus_v,
                 z = diff.x * u_minus_v * u_minus_v;
            return {x, z};
        }
        Point mul(const Point& p, u64 k) const {
            Point p0 = p, p1 = dbl(p);
            _for_r (b, std::bit_width(k) - 1) {
                Point tmp = add(p1, p0, p);
                if ((k >> b) & 1) {
                    p1 = dbl(p1);
                    p0 = tmp;
                }
                else {
                    p0 = dbl(p0);
                    p1 = tmp;
                }
            }
            return p0;
        }

    private:
        mint _a24;
    };
    Vec<u64> ecm_blocks(int smooth_bound) {
        Vec<bool> sieve(smooth_bound + 1, true);
        Vec<u64> blocks{1};
        _for (p, 2, smooth_bound + 1) {
            if (sieve[p]) {
                int pw = p;
                while (pw <= smooth_bound) {
                    if (blocks.back() <= std::numeric_limits<u64>::max() / p)
                        blocks.back() *= p;
                    else
                        blocks.push_back(p);
                    pw *= p;
                }
                _for (i, p * p, smooth_bound + 1, p)
                    sieve[i] = false;
            }
        }
        return blocks;
    }
    template <typename mint, typename T = typename mint::int_type>
    T ecm_modint() {
        constexpr int B1 = 400, B2 = 3000;
        static const Vec<u64> blocks = ecm_blocks(B1);
        while (true) {
            auto [curve, point] = MontgomeryCurve<mint>::random_curve_and_point();
            T f = 1;
            foreach (blk, blocks) {
                auto new_point = curve.mul(point, blk);
                f = new_point.check();
                if (f != 1) {
                    if (f != mint::mod())
                        return f;
                    break;
                }
                point = new_point;
            }
            if (f == mint::mod())
                continue;
            auto six = curve.dbl(curve.add(curve.dbl(point), point, point));
            auto q0 = six, q1 = curve.dbl(six);
            _for (i, 6, B1, 6) {
                q0 = curve.add(q1, six, q0);
                std::swap(q0, q1);
            }
            mint xprod(1);
            mint x_norm = point.x / point.z;
            _for (i, B1 / 6 * 6, B2, 6) {
                xprod *= q0.x - q0.z * x_norm;
                if (i % 300 == 0) {
                    f = std::gcd(xprod.val(), mint::mod());
                    if (f != 1) {
                        if (f != mint::mod())
                            return f;
                        break;
                    }
                }
                q0 = curve.add(q1, six, q0);
                std::swap(q0, q1);
            }
            if (f == 1) {
                f = std::gcd(xprod.val(), mint::mod());
                if (f != 1 && f != mint::mod())
                    return f;
            }
        }
    }
    template <typename T>
    T ecm(T n) {
        _for (k, 2, std::bit_width(n)) {
            T r = roundl(powl(n, 1.0L / k)), pw = r;
            _for (k - 1)
                pw *= r;
            if (pw == n)
                return r;
        }
        SetMod(T, n);
        return ecm_modint<mint>();
    }
    template <typename mint, typename T = typename mint::int_type>
    T pollard_rho_modint() {
        const T n = mint::mod();
        constexpr T m = std::numeric_limits<T>::digits;
        T r = 1, g;
        mint c, y, q, x, ys;
        do {
            c++;
            y = 2;
            q = 1;
            g = 1;
            do {
                x = y;
                _for (r)
                    y = y * y + c;
                ys = y;
                _for (i, r) {
                    y = y * y + c;
                    q *= y - x;
                    if ((i + 1) % m == 0) {
                        g = std::gcd(q.val(), n);
                        if (g != 1) {
                            break;
                        }
                        ys = y;
                    }
                }
                if (g == 1 && r % m != 0)
                    g = std::gcd(q.val(), n);
                r *= 2;
            } while (g == 1);
            if (g == n) {
                do {
                    ys = ys * ys + c;
                    g = std::gcd((ys - x).val(), n);
                } while (g == 1);
            }
        } while (g == n);
        return g;
    }
    template <typename T>
    T pollard_rho(T n) {
        SetMod(T, n);
        return pollard_rho_modint<mint>();
    }
    template <typename T>
    void factorize_work(FactorizationResult<T>& r) {
        T n = pop(r.f);
        T f = prime_or_factor(n);
        if (f == 1) {
            r.pf.push_back(n);
            return;
        }
        if (f == 0) {
            if (n < (1 << 30))
                f = pollard_rho<u32>(n);
            else if (n < (1ULL << 59))
                f = pollard_rho<u64>(n);
            else
                f = ecm<u64>(n);
        }
        r.f.push_back(f);
        r.f.push_back(n / f);
    }
} // namespace impl
template <Unsigned T>
Vec<T> factorize(T n) {
    if (n <= 1)
        return {};
    int twos = std::countr_zero(n);
    impl::FactorizationResult<T> r;
    r.pf.insert(r.pf.end(), twos, 2);
    if ((n & (n - 1)) == 0)
        return r.pf;
    r.f.push_back(n >> twos);
    while (!r.f.empty())
        impl::factorize_work(r);
    sort(r.pf);
    return r.pf;
}
template <Unsigned T>
Vec<std::pair<T, int>> factorize_pair(T n) {
    Vec<std::pair<T, int>> r;
    foreach (p, factorize(n))
        if (r.empty() || r.back().first != p)
            r.eb(p, 1);
        else
            r.back().second++;
    return r;
}
vi factorize(int n, const vi& lpf) {
    vi r;
    while (n > 1) {
        int p = lpf[n];
        r.eb(p);
        n /= p;
    }
    return r;
}
Vec<pi> factorize_pair(int n, const vi& lpf) {
    Vec<pi> r;
    while (n > 1) {
        int p = lpf[n], c = 0;
        while (n % p == 0) {
            n /= p;
            c++;
        }
        r.eb(p, c);
    }
    return r;
}
