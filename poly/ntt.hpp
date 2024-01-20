#pragma once
#include "math/primitive_root_constexpr.hpp"
#include "modint/montgomery.hpp"

template <typename mint>
struct NTT {
    static constexpr u32 mod = mint::mod();
    static constexpr mint pr = primitive_root_constexpr(mod);
    static constexpr int lvl = __builtin_ctz(mod - 1);
    mint dw[lvl], dy[lvl];
    constexpr NTT() {
        if (lvl < 3)
            return;
        mint w[lvl], y[lvl];
        w[lvl - 1] = power(pr, (mod - 1) / (1 << lvl));
        y[lvl - 1] = w[lvl - 1].inv();
        _for_r (i, 1, lvl - 1)
            w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        _for (i, 3, lvl) {
            dw[i] = dw[i - 1] * y[i - 2] * w[i];
            dy[i] = dy[i - 1] * w[i - 2] * y[i];
        }
    }
};
namespace ArbitraryNTT {
    constexpr u32 m0 = 167772161;
    constexpr u32 m1 = 469762049;
    constexpr u32 m2 = 754974721;
    using mint0 = MMInt<m0>;
    using mint1 = MMInt<m1>;
    using mint2 = MMInt<m2>;
    constexpr u32 r01 = mint1(m0).inv().val();
    constexpr u32 r02 = mint2(m0).inv().val();
    constexpr u32 r12 = mint2(m1).inv().val();
    constexpr u32 r02r12 = u64(r02) * r12 % m2;
    constexpr u64 w1 = m0;
    constexpr u64 w2 = u64(m0) * m1;
    template <typename T, u64 w1, u64 w2>
    void crt(auto&& c0, auto&& c1, auto&& c2, auto&& r) {
        _for (i, len(r)) {
            u64 n1 = c1[i].val(), n2 = c2[i].val(), a = c0[i].val();
            u64 b = (n1 + m1 - a) * r01 % m1;
            u64 c = ((n2 + m2 - a) * r02r12 + (m2 - b) * r12) % m2;
            r[i] = a + b * w1 + T(c) * w2;
        }
    }
} // namespace ArbitraryNTT
template <typename mint>
void fft4(vc<mint>& a, int k) {
    constexpr NTT<mint> ntt;
    if (len(a) <= 1)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        return;
    }
    if (k & 1) {
        int v = 1 << (k - 1);
        _for (j, v)
            a[j] += std::exchange(a[j + v], a[j] - a[j + v]);
    }
    int u = 1 << (2 + (k & 1));
    int v = 1 << (k - 2 - (k & 1));
    mint one(1);
    mint imag = ntt.dw[1];
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
        mint ww = one, xx = one * ntt.dw[2], wx = one;
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
            xx *= ntt.dw[__builtin_ctz(jh += 4)];
        }
        u <<= 2;
        v >>= 2;
    }
}
template <typename mint>
void ifft4(vc<mint>& a, int k) {
    constexpr NTT<mint> ntt;
    if (len(a) <= 1)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        return;
    }
    int u = 1 << (k - 2);
    int v = 1;
    mint one(1);
    mint imag = ntt.dy[1];
    while (u) {
        // jh = 0
        {
            int j0 = 0;
            int j1 = v;
            int j2 = v + v;
            int j3 = j2 + v;
            for (; j0 < v; j0++, j1++, j2++, j3++) {
                mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
            }
        }
        // jh >= 1
        mint ww = one, xx = one * ntt.dy[2], yy = one;
        u <<= 2;
        for (int jh = 4; jh < u;) {
            ww = xx * xx, yy = xx * imag;
            int j0 = jh * v;
            int je = j0 + v;
            int j2 = je + v;
            for (; j0 < je; j0++, j2++) {
                mint t0 = a[j0], t1 = a[j0 + v], t2 = a[j2], t3 = a[j2 + v];
                mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                a[j0 + v] = t0m1 + t2m3, a[j2 + v] = (t0m1 - t2m3) * ww;
            }
            xx *= ntt.dy[__builtin_ctz(jh += 4)];
        }
        u >>= 4;
        v <<= 2;
    }
    if (k & 1) {
        u = 1 << (k - 1);
        _for (j, u)
            a[j] += std::exchange(a[j + u], a[j] - a[j + u]);
    }
}
template <typename mint>
void ntt(vc<mint>& a) {
    if (len(a) <= 1)
        return;
    fft4(a, __builtin_ctz(len(a)));
}
template <typename mint>
void intt(vc<mint>& a) {
    if (len(a) <= 1)
        return;
    ifft4(a, __builtin_ctz(len(a)));
    mint iv = mint(len(a)).inv();
    foreach (x, a)
        x *= iv;
}
template <typename mint>
void ntt_doubling(vc<mint>& a) {
    constexpr NTT<mint> ntt;
    int n = len(a);
    auto b{a};
    intt(b);
    mint r = 1, zeta = power(ntt.pr, (ntt.mod - 1) / (n << 1));
    _for (i, n)
        b[i] *= r, r *= zeta;
    ntt(b);
    a.insert(a.end(), all(b));
}