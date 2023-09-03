#include "barrett-reduction.hpp"

template <int id>
struct ArbitraryModIntBase {
    using Z = ArbitraryModIntBase;

    ArbitraryModIntBase() = default;

    ArbitraryModIntBase(i64 y) {
        int z = y % get_mod();
        if (z < 0)
            z += get_mod();
        x = z;
    }
    Z& operator+=(const Z& p) {
        x += p.x;
        if (x >= get_mod())
            x -= get_mod();
        return *this;
    }
    Z& operator-=(const Z& p) {
        x += get_mod() - p.x;
        if (x >= get_mod())
            x -= get_mod();
        return *this;
    }
    Z& operator*=(const Z& p) {
        x = rem((unsigned long long)x * p.x);
        return *this;
    }
    Z& operator/=(const Z& p) {
        return *this *= p.inverse();
    }
    Z operator-() const { return Z(-x); }
    Z operator+() const { return *this; }
    friend Z operator+(Z lhs, const Z& rhs) {
        return lhs += rhs;
    }
    friend Z operator-(Z lhs, const Z& rhs) {
        return lhs -= rhs;
    }
    friend Z operator*(Z lhs, const Z& rhs) {
        return lhs *= rhs;
    }
    friend Z operator/(Z lhs, const Z& rhs) {
        return lhs /= rhs;
    }
    Z inverse() const {
        int a = x, b = get_mod(), u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return Z(u);
    }
    Z pow(i64 n) const {
        Z ret(1), mul(x);
        while (n > 0) {
            if (n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    [[nodiscard]] int val() const { return x; }
    static int& get_mod() {
        static int mod = 0;
        return mod;
    }
    static void set_mod(int p) {
        assert(0 < p && p <= (1 << 30) - 1);
        get_mod() = p;
        barrett() = Barrett(p);
    }

private:
    int x = 0;

    u32 rem(u64 p) { return barrett().rem(p); }
    static Barrett& barrett() {
        static Barrett b;
        return b;
    }
};

using ArbitraryModInt = ArbitraryModIntBase<-1>;