#pragma once
#include "../modint/montgomery.hpp"
#include "../rng.hpp"

namespace impl {
    template <typename T>
    struct FactorizationResult {
        std::vector<T> factors, prime_factors;
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
        using ctx = DynamicMontgomeryReductionContext<u32>;
        auto _guard = ctx::set_mod(n);
        using mint = MontgomeryModInt<ctx>;
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
        using ctx = DynamicMontgomeryReductionContext<u64>;
        auto _guard = ctx::set_mod(n);
        using mint = MontgomeryModInt<ctx>;
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
            T check() const {
                return std::gcd(z.val(), mint::mod());
            }
        };
        MontgomeryCurve(mint a): _a24((a + 2) / 4) {}
        static std::pair<MontgomeryCurve, Point> random_curve_and_point() {
            while (true) {
                mint a(rnd()),
                  x(rnd()),
                  m1(1),
                  y2 = x * (x * (x + a) + m1);
                if (jacobi(y2.val(), mint::mod()) == 1) {
                    return {MontgomeryCurve(a), Point{x, m1}};
                }
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
            for (int b = std::bit_width(k) - 2; b >= 0; b--) {
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
    std::vector<u64> ecm_blocks(int smooth_bound) {
        std::vector<bool> sieve(smooth_bound + 1, true);
        std::vector<u64> blocks{1};
        for (int p = 2; p <= smooth_bound; p++) {
            if (sieve[p]) {
                int pw = p;
                while (pw <= smooth_bound) {
                    if (blocks.back() <= std::numeric_limits<u64>::max() / p) {
                        blocks.back() *= p;
                    }
                    else {
                        blocks.push_back(p);
                    }
                    pw *= p;
                }
                for (int i = p * p; i <= smooth_bound; i += p) {
                    sieve[i] = false;
                }
            }
        }
        return blocks;
    }
    template <typename mint, typename T = typename mint::int_type>
    T ecm_modint() {
        constexpr int B1 = 400, B2 = 3000;
        static const std::vector<u64> blocks = ecm_blocks(B1);
        while (true) {
            auto [curve, point] = MontgomeryCurve<mint>::random_curve_and_point();
            T f = 1;
            for (u64 blk: blocks) {
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
            for (int i = 6; i < B1; i += 6) {
                q0 = curve.add(q1, six, q0);
                std::swap(q0, q1);
            }
            mint xprod(1);
            mint x_norm = point.x / point.z;
            for (int i = B1 / 6 * 6; i < B2; i += 6) {
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
            if (pw == n) {
                return r;
            }
        }
        using ctx = DynamicMontgomeryReductionContext<T>;
        auto _guard = ctx::set_mod(n);
        using mint = MontgomeryModInt<ctx>;
        return ecm_modint<mint>();
    }
    template <typename mint, typename T = typename mint::int_type>
    T pollard_rho_modint() {
        const T n = mint::mod();
        constexpr T m = std::numeric_limits<T>::digits;
        T r = 1, g;
        mint c(0), y, q, x, ys;
        do {
            ++c;
            y = mint(2);
            q = mint(1);
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
        using ctx = DynamicMontgomeryReductionContext<T>;
        auto _guard = ctx::set_mod(n);
        using mint = MontgomeryModInt<ctx>;
        return pollard_rho_modint<mint>();
    }
    template <typename T>
    void factorize_work(FactorizationResult<T>& result) {
        T n = result.factors.back();
        result.factors.pop_back();
        T f = prime_or_factor(n);
        if (f == 1) {
            result.prime_factors.push_back(n);
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
        result.factors.push_back(f);
        result.factors.push_back(n / f);
    }
} // namespace impl

template <Unsigned T>
Vec<T> factorize(T n) {
    if (n <= 1)
        return {};
    int twos = std::countr_zero(n);
    impl::FactorizationResult<T> result;
    result.prime_factors.insert(result.prime_factors.end(), twos, 2);
    if ((n & (n - 1)) == 0)
        return result.prime_factors;
    result.factors.push_back(n >> twos);
    while (!result.factors.empty())
        impl::factorize_work(result);
    sort(result.prime_factors);
    return result.prime_factors;
}