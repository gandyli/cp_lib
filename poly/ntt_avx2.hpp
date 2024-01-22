#pragma once
#include "math/primitive_root_constexpr.hpp"
#include "poly/arbitrary_ntt.hpp"
#include "modint/simd_montgomery.hpp"

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
constexpr int FFT_BUF_SIZE = 1 << 23;
u32 _buf1[FFT_BUF_SIZE] __attribute__((aligned(64)));
u32 _buf2[FFT_BUF_SIZE] __attribute__((aligned(64)));
template <typename mint>
__attribute__((target("avx2"))) void ntt(mint* a, int n) {
    constexpr NTT<mint> ntt;
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        return;
    }
    if (k & 1) {
        int v = 1 << (k - 1);
        if (v < 8) {
            _for (j, v)
                a[j] += std::exchange(a[j + v], a[j] - a[j + v]);
        }
        else {
            const __m256i m0 = _mm256_set1_epi32(0);
            const __m256i m2 = _mm256_set1_epi32(ntt.mod + ntt.mod);
            int j0 = 0;
            int j1 = v;
            for (; j0 < v; j0 += 8, j1 += 8) {
                __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                __m256i naj = montgomery_add_256(T0, T1, m2, m0);
                __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
                _mm256_storeu_si256((__m256i*)(a + j0), naj);
                _mm256_storeu_si256((__m256i*)(a + j1), najv);
            }
        }
    }
    int u = 1 << (2 + (k & 1));
    int v = 1 << (k - 2 - (k & 1));
    mint one(1);
    mint imag = ntt.dw[1];
    while (v) {
        if (v == 1) {
            mint ww = one, xx = one, wx = one;
            for (int jh = 0; jh < u;) {
                ww = xx * xx, wx = ww * xx;
                mint t0 = a[jh + 0], t1 = a[jh + 1] * xx;
                mint t2 = a[jh + 2] * ww, t3 = a[jh + 3] * wx;
                mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                a[jh + 0] = t0p2 + t1p3, a[jh + 1] = t0p2 - t1p3;
                a[jh + 2] = t0m2 + t1m3, a[jh + 3] = t0m2 - t1m3;
                xx *= ntt.dw[__builtin_ctz((jh += 4))];
            }
        }
        else if (v == 4) {
            const __m128i m0 = _mm_set1_epi32(0);
            const __m128i m1 = _mm_set1_epi32(ntt.mod);
            const __m128i m2 = _mm_set1_epi32(ntt.mod + ntt.mod);
            const __m128i r = _mm_set1_epi32(mint::mr().inv_base(mint::mod()));
            const __m128i Imag = _mm_set1_epi32(imag.raw());
            mint ww = one, xx = one, wx = one;
            for (int jh = 0; jh < u;) {
                if (jh == 0) {
                    int j0 = 0;
                    int j1 = v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = v;
                    for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
                        const __m128i T0 = _mm_loadu_si128((__m128i*)(a + j0));
                        const __m128i T1 = _mm_loadu_si128((__m128i*)(a + j1));
                        const __m128i T2 = _mm_loadu_si128((__m128i*)(a + j2));
                        const __m128i T3 = _mm_loadu_si128((__m128i*)(a + j3));
                        const __m128i T0P2 = montgomery_add_128(T0, T2, m2, m0);
                        const __m128i T1P3 = montgomery_add_128(T1, T3, m2, m0);
                        const __m128i T0M2 = montgomery_sub_128(T0, T2, m2, m0);
                        const __m128i T1M3 = montgomery_mul_128(
                          montgomery_sub_128(T1, T3, m2, m0), Imag, r, m1);
                        _mm_storeu_si128((__m128i*)(a + j0),
                                         montgomery_add_128(T0P2, T1P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j1),
                                         montgomery_sub_128(T0P2, T1P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j2),
                                         montgomery_add_128(T0M2, T1M3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j3),
                                         montgomery_sub_128(T0M2, T1M3, m2, m0));
                    }
                }
                else {
                    ww = xx * xx, wx = ww * xx;
                    const __m128i WW = _mm_set1_epi32(ww.raw());
                    const __m128i WX = _mm_set1_epi32(wx.raw());
                    const __m128i XX = _mm_set1_epi32(xx.raw());
                    int j0 = jh * v;
                    int j1 = j0 + v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = j1;
                    for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
                        const __m128i T0 = _mm_loadu_si128((__m128i*)(a + j0));
                        const __m128i T1 = _mm_loadu_si128((__m128i*)(a + j1));
                        const __m128i T2 = _mm_loadu_si128((__m128i*)(a + j2));
                        const __m128i T3 = _mm_loadu_si128((__m128i*)(a + j3));
                        const __m128i MT1 = montgomery_mul_128(T1, XX, r, m1);
                        const __m128i MT2 = montgomery_mul_128(T2, WW, r, m1);
                        const __m128i MT3 = montgomery_mul_128(T3, WX, r, m1);
                        const __m128i T0P2 = montgomery_add_128(T0, MT2, m2, m0);
                        const __m128i T1P3 = montgomery_add_128(MT1, MT3, m2, m0);
                        const __m128i T0M2 = montgomery_sub_128(T0, MT2, m2, m0);
                        const __m128i T1M3 = montgomery_mul_128(
                          montgomery_sub_128(MT1, MT3, m2, m0), Imag, r, m1);
                        _mm_storeu_si128((__m128i*)(a + j0),
                                         montgomery_add_128(T0P2, T1P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j1),
                                         montgomery_sub_128(T0P2, T1P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j2),
                                         montgomery_add_128(T0M2, T1M3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j3),
                                         montgomery_sub_128(T0M2, T1M3, m2, m0));
                    }
                }
                xx *= ntt.dw[__builtin_ctz((jh += 4))];
            }
        }
        else {
            const __m256i m0 = _mm256_set1_epi32(0);
            const __m256i m1 = _mm256_set1_epi32(ntt.mod);
            const __m256i m2 = _mm256_set1_epi32(ntt.mod + ntt.mod);
            const __m256i r = _mm256_set1_epi32(mint::mr().inv_base(mint::mod()));
            const __m256i Imag = _mm256_set1_epi32(imag.raw());
            mint ww = one, xx = one, wx = one;
            for (int jh = 0; jh < u;) {
                if (jh == 0) {
                    int j0 = 0;
                    int j1 = v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = v;
                    for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
                        const __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                        const __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                        const __m256i T2 = _mm256_loadu_si256((__m256i*)(a + j2));
                        const __m256i T3 = _mm256_loadu_si256((__m256i*)(a + j3));
                        const __m256i T0P2 = montgomery_add_256(T0, T2, m2, m0);
                        const __m256i T1P3 = montgomery_add_256(T1, T3, m2, m0);
                        const __m256i T0M2 = montgomery_sub_256(T0, T2, m2, m0);
                        const __m256i T1M3 = montgomery_mul_256(
                          montgomery_sub_256(T1, T3, m2, m0), Imag, r, m1);
                        _mm256_storeu_si256((__m256i*)(a + j0),
                                            montgomery_add_256(T0P2, T1P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j1),
                                            montgomery_sub_256(T0P2, T1P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j2),
                                            montgomery_add_256(T0M2, T1M3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j3),
                                            montgomery_sub_256(T0M2, T1M3, m2, m0));
                    }
                }
                else {
                    ww = xx * xx, wx = ww * xx;
                    const __m256i WW = _mm256_set1_epi32(ww.raw());
                    const __m256i WX = _mm256_set1_epi32(wx.raw());
                    const __m256i XX = _mm256_set1_epi32(xx.raw());
                    int j0 = jh * v;
                    int j1 = j0 + v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = j1;
                    for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
                        const __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                        const __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                        const __m256i T2 = _mm256_loadu_si256((__m256i*)(a + j2));
                        const __m256i T3 = _mm256_loadu_si256((__m256i*)(a + j3));
                        const __m256i MT1 = montgomery_mul_256(T1, XX, r, m1);
                        const __m256i MT2 = montgomery_mul_256(T2, WW, r, m1);
                        const __m256i MT3 = montgomery_mul_256(T3, WX, r, m1);
                        const __m256i T0P2 = montgomery_add_256(T0, MT2, m2, m0);
                        const __m256i T1P3 = montgomery_add_256(MT1, MT3, m2, m0);
                        const __m256i T0M2 = montgomery_sub_256(T0, MT2, m2, m0);
                        const __m256i T1M3 = montgomery_mul_256(
                          montgomery_sub_256(MT1, MT3, m2, m0), Imag, r, m1);
                        _mm256_storeu_si256((__m256i*)(a + j0),
                                            montgomery_add_256(T0P2, T1P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j1),
                                            montgomery_sub_256(T0P2, T1P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j2),
                                            montgomery_add_256(T0M2, T1M3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j3),
                                            montgomery_sub_256(T0M2, T1M3, m2, m0));
                    }
                }
                xx *= ntt.dw[__builtin_ctz((jh += 4))];
            }
        }
        u <<= 2;
        v >>= 2;
    }
}
template <typename mint>
__attribute__((target("avx2"))) void intt(mint* a, int n, int normalize = true) {
    constexpr NTT<mint> ntt;
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        if (normalize) {
            a[0] *= mint(2).inv();
            a[1] *= mint(2).inv();
        }
        return;
    }
    int u = 1 << (k - 2);
    int v = 1;
    mint one(1);
    mint imag = ntt.dy[1];
    while (u) {
        if (v == 1) {
            mint ww = one, xx = one, yy = one;
            u <<= 2;
            for (int jh = 0; jh < u;) {
                ww = xx * xx, yy = xx * imag;
                mint t0 = a[jh + 0], t1 = a[jh + 1];
                mint t2 = a[jh + 2], t3 = a[jh + 3];
                mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                a[jh + 0] = t0p1 + t2p3, a[jh + 2] = (t0p1 - t2p3) * ww;
                a[jh + 1] = t0m1 + t2m3, a[jh + 3] = (t0m1 - t2m3) * ww;
                xx *= ntt.dy[__builtin_ctz(jh += 4)];
            }
        }
        else if (v == 4) {
            const __m128i m0 = _mm_set1_epi32(0);
            const __m128i m1 = _mm_set1_epi32(ntt.mod);
            const __m128i m2 = _mm_set1_epi32(ntt.mod + ntt.mod);
            const __m128i r = _mm_set1_epi32(mint::mr().inv_base(mint::mod()));
            const __m128i Imag = _mm_set1_epi32(imag.raw());
            mint ww = one, xx = one, yy = one;
            u <<= 2;
            for (int jh = 0; jh < u;) {
                if (jh == 0) {
                    int j0 = 0;
                    int j1 = v;
                    int j2 = v + v;
                    int j3 = j2 + v;
                    for (; j0 < v; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
                        const __m128i T0 = _mm_loadu_si128((__m128i*)(a + j0));
                        const __m128i T1 = _mm_loadu_si128((__m128i*)(a + j1));
                        const __m128i T2 = _mm_loadu_si128((__m128i*)(a + j2));
                        const __m128i T3 = _mm_loadu_si128((__m128i*)(a + j3));
                        const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
                        const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
                        const __m128i T0M1 = montgomery_sub_128(T0, T1, m2, m0);
                        const __m128i T2M3 = montgomery_mul_128(
                          montgomery_sub_128(T2, T3, m2, m0), Imag, r, m1);
                        _mm_storeu_si128((__m128i*)(a + j0),
                                         montgomery_add_128(T0P1, T2P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j2),
                                         montgomery_sub_128(T0P1, T2P3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j1),
                                         montgomery_add_128(T0M1, T2M3, m2, m0));
                        _mm_storeu_si128((__m128i*)(a + j3),
                                         montgomery_sub_128(T0M1, T2M3, m2, m0));
                    }
                }
                else {
                    ww = xx * xx, yy = xx * imag;
                    const __m128i WW = _mm_set1_epi32(ww.raw());
                    const __m128i XX = _mm_set1_epi32(xx.raw());
                    const __m128i YY = _mm_set1_epi32(yy.raw());
                    int j0 = jh * v;
                    int j1 = j0 + v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = j1;
                    for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
                        const __m128i T0 = _mm_loadu_si128((__m128i*)(a + j0));
                        const __m128i T1 = _mm_loadu_si128((__m128i*)(a + j1));
                        const __m128i T2 = _mm_loadu_si128((__m128i*)(a + j2));
                        const __m128i T3 = _mm_loadu_si128((__m128i*)(a + j3));
                        const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
                        const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
                        const __m128i T0M1 = montgomery_mul_128(
                          montgomery_sub_128(T0, T1, m2, m0), XX, r, m1);
                        __m128i T2M3 = montgomery_mul_128(
                          montgomery_sub_128(T2, T3, m2, m0), YY, r, m1);
                        _mm_storeu_si128((__m128i*)(a + j0),
                                         montgomery_add_128(T0P1, T2P3, m2, m0));
                        _mm_storeu_si128(
                          (__m128i*)(a + j2),
                          montgomery_mul_128(montgomery_sub_128(T0P1, T2P3, m2, m0), WW,
                                             r, m1));
                        _mm_storeu_si128((__m128i*)(a + j1),
                                         montgomery_add_128(T0M1, T2M3, m2, m0));
                        _mm_storeu_si128(
                          (__m128i*)(a + j3),
                          montgomery_mul_128(montgomery_sub_128(T0M1, T2M3, m2, m0), WW,
                                             r, m1));
                    }
                }
                xx *= ntt.dy[__builtin_ctz(jh += 4)];
            }
        }
        else {
            const __m256i m0 = _mm256_set1_epi32(0);
            const __m256i m1 = _mm256_set1_epi32(ntt.mod);
            const __m256i m2 = _mm256_set1_epi32(ntt.mod + ntt.mod);
            const __m256i r = _mm256_set1_epi32(mint::mr().inv_base(mint::mod()));
            const __m256i Imag = _mm256_set1_epi32(imag.raw());
            mint ww = one, xx = one, yy = one;
            u <<= 2;
            for (int jh = 0; jh < u;) {
                if (jh == 0) {
                    int j0 = 0;
                    int j1 = v;
                    int j2 = v + v;
                    int j3 = j2 + v;
                    for (; j0 < v; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
                        const __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                        const __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                        const __m256i T2 = _mm256_loadu_si256((__m256i*)(a + j2));
                        const __m256i T3 = _mm256_loadu_si256((__m256i*)(a + j3));
                        const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
                        const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
                        const __m256i T0M1 = montgomery_sub_256(T0, T1, m2, m0);
                        const __m256i T2M3 = montgomery_mul_256(
                          montgomery_sub_256(T2, T3, m2, m0), Imag, r, m1);
                        _mm256_storeu_si256((__m256i*)(a + j0),
                                            montgomery_add_256(T0P1, T2P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j2),
                                            montgomery_sub_256(T0P1, T2P3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j1),
                                            montgomery_add_256(T0M1, T2M3, m2, m0));
                        _mm256_storeu_si256((__m256i*)(a + j3),
                                            montgomery_sub_256(T0M1, T2M3, m2, m0));
                    }
                }
                else {
                    ww = xx * xx, yy = xx * imag;
                    const __m256i WW = _mm256_set1_epi32(ww.raw());
                    const __m256i XX = _mm256_set1_epi32(xx.raw());
                    const __m256i YY = _mm256_set1_epi32(yy.raw());
                    int j0 = jh * v;
                    int j1 = j0 + v;
                    int j2 = j1 + v;
                    int j3 = j2 + v;
                    int je = j1;
                    for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
                        const __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                        const __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                        const __m256i T2 = _mm256_loadu_si256((__m256i*)(a + j2));
                        const __m256i T3 = _mm256_loadu_si256((__m256i*)(a + j3));
                        const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
                        const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
                        const __m256i T0M1 = montgomery_mul_256(
                          montgomery_sub_256(T0, T1, m2, m0), XX, r, m1);
                        const __m256i T2M3 = montgomery_mul_256(
                          montgomery_sub_256(T2, T3, m2, m0), YY, r, m1);
                        _mm256_storeu_si256((__m256i*)(a + j0),
                                            montgomery_add_256(T0P1, T2P3, m2, m0));
                        _mm256_storeu_si256(
                          (__m256i*)(a + j2),
                          montgomery_mul_256(montgomery_sub_256(T0P1, T2P3, m2, m0), WW,
                                             r, m1));
                        _mm256_storeu_si256((__m256i*)(a + j1),
                                            montgomery_add_256(T0M1, T2M3, m2, m0));
                        _mm256_storeu_si256(
                          (__m256i*)(a + j3),
                          montgomery_mul_256(montgomery_sub_256(T0M1, T2M3, m2, m0), WW,
                                             r, m1));
                    }
                }
                xx *= ntt.dy[__builtin_ctz(jh += 4)];
            }
        }
        u >>= 4;
        v <<= 2;
    }
    if (k & 1) {
        v = 1 << (k - 1);
        if (v < 8) {
            _for (j, v)
                a[j] += std::exchange(a[j + v], a[j] - a[j + v]);
        }
        else {
            const __m256i m0 = _mm256_set1_epi32(0);
            const __m256i m2 = _mm256_set1_epi32(ntt.mod + ntt.mod);
            int j0 = 0;
            int j1 = v;
            for (; j0 < v; j0 += 8, j1 += 8) {
                const __m256i T0 = _mm256_loadu_si256((__m256i*)(a + j0));
                const __m256i T1 = _mm256_loadu_si256((__m256i*)(a + j1));
                __m256i naj = montgomery_add_256(T0, T1, m2, m0);
                __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
                _mm256_storeu_si256((__m256i*)(a + j0), naj);
                _mm256_storeu_si256((__m256i*)(a + j1), najv);
            }
        }
    }
    if (normalize) {
        mint invn = mint(n).inv();
        _for (i, n)
            a[i] *= invn;
    }
}
template <typename mint>
__attribute__((target("avx2"))) void inplace_multiply(int l1, int l2, int zero_padding = true) {
    constexpr NTT<mint> ntt;
    int l = l1 + l2 - 1;
    int M = 4;
    while (M < l)
        M <<= 1;
    if (zero_padding) {
        _for (i, l1, M)
            _buf1[i] = 0;
        _for (i, l2, M)
            _buf2[i] = 0;
    }
    const __m256i m0 = _mm256_set1_epi32(0);
    const __m256i m1 = _mm256_set1_epi32(ntt.mod);
    const __m256i r = _mm256_set1_epi32(mint::mr().inv_base(mint::mod()));
    const __m256i N2 = _mm256_set1_epi32(mint::mr().mbase2());
    _for (i, 0, l1, 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)(_buf1 + i));
        __m256i b = montgomery_mul_256(a, N2, r, m1);
        _mm256_storeu_si256((__m256i*)(_buf1 + i), b);
    }
    _for (i, 0, l2, 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)(_buf2 + i));
        __m256i b = montgomery_mul_256(a, N2, r, m1);
        _mm256_storeu_si256((__m256i*)(_buf2 + i), b);
    }
    ::ntt((mint*)_buf1, M);
    ::ntt((mint*)_buf2, M);
    _for (i, 0, M, 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)(_buf1 + i));
        __m256i b = _mm256_loadu_si256((__m256i*)(_buf2 + i));
        __m256i c = montgomery_mul_256(a, b, r, m1);
        _mm256_storeu_si256((__m256i*)(_buf1 + i), c);
    }
    intt((mint*)_buf1, M, false);
    const __m256i INVM = _mm256_set1_epi32((mint(M).inv()).raw());
    _for (i, 0, l, 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)(_buf1 + i));
        __m256i b = montgomery_mul_256(a, INVM, r, m1);
        __m256i c = my256_mulhi_epu32(my256_mullo_epu32(b, r), m1);
        __m256i d = _mm256_and_si256(_mm256_cmpgt_epi32(c, m0), m1);
        __m256i e = _mm256_sub_epi32(d, c);
        _mm256_storeu_si256((__m256i*)(_buf1 + i), e);
    }
}
template <typename mint>
void ntt_avx2(vc<mint>& a) {
    int M = len(a);
    _for (i, M)
        (mint*)_buf1[i] = a[i];
    ntt((mint*)_buf1, M);
    _for (i, M)
        a[i] = (mint*)_buf1[i];
}
template <typename mint>
void intt_avx2(vc<mint>& a) {
    int M = len(a);
    _for (i, M)
        (mint*)_buf1[i] = a[i];
    intt((mint*)_buf1, M, true);
    _for (i, M)
        a[i] = (mint*)_buf1[i];
}
template <Modint mint>
vc<mint> convolution_ntt_avx2(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    int k = get_lg(n + m - 1), sz = 1 << k;
    _for (i, len(a))
        ((mint*)_buf1)[i] = a[i];
    _for (i, len(a), sz)
        ((mint*)_buf1)[i] = 0;
    _for (i, len(b))
        ((mint*)_buf2)[i] = b[i];
    _for (i, len(b), sz)
        ((mint*)_buf2)[i] = 0;
    ntt((mint*)_buf1, sz);
    ntt((mint*)_buf2, sz);
    _for (i, sz)
        ((mint*)_buf1)[i] *= ((mint*)_buf2)[i];
    intt((mint*)_buf1, sz, false);
    vc<mint> s(n + m - 1);
    mint iv = mint(sz).inv();
    _for (i, n + m - 1)
        s[i] = ((mint*)_buf1)[i] * iv;
    return s;
}
template <typename mint>
void ntt_doubling_avx2(vc<mint>& a) {
    constexpr NTT<mint> ntt;
    int n = len(a);
    _for (i, n)
        ((mint*)_buf1)[i] = a[i];
    intt((mint*)_buf1, n);
    mint r = 1, zeta = power(ntt.pr, (ntt.mod - 1) / (n << 1));
    _for (i, n)
        ((mint*)_buf1)[i] *= r, r *= zeta;
    ::ntt((mint*)_buf1, n);
    a.resize(2 * n);
    _for (i, n)
        a[n + i] = ((mint*)_buf1)[i];
}
template <Modint mint>
vc<mint> convolution_garner_avx2(const vc<mint>& a, const vc<mint>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    _for (i, n) {
        a0[i] = a[i].val();
        a1[i] = a[i].val();
        a2[i] = a[i].val();
    }
    _for (i, m) {
        b0[i] = b[i].val();
        b1[i] = b[i].val();
        b2[i] = b[i].val();
    }
    auto c0 = convolution_ntt_avx2(a0, b0);
    auto c1 = convolution_ntt_avx2(a1, b1);
    auto c2 = convolution_ntt_avx2(a2, b2);
    vc<mint> r(n + m - 1);
    crt<u64>(c0, c1, c2, r, w1 % mint::mod(), w2 % mint::mod());
    return r;
}