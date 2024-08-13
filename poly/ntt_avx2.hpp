#pragma once
#include <immintrin.h>
#include "math/primitive_root_constexpr.hpp"
#include "modint/montgomery_reduction.hpp"

#define NTT_AVX2

// https://judge.yosupo.jp/submission/199421
using I256 = __m256i;
void store256(void* p, I256 x) { _mm256_store_si256((I256*)p, x); }
I256 load256(const void* p) { return _mm256_load_si256((const I256*)p); }
I256 shrk32(I256 x, I256 M) { return _mm256_min_epu32(x, _mm256_sub_epi32(x, M)); }
I256 dilt32(I256 x, I256 M) { return _mm256_min_epu32(x, _mm256_add_epi32(x, M)); }
I256 Ladd32(I256 x, I256 y, I256) { return _mm256_add_epi32(x, y); }
I256 Lsub32(I256 x, I256 y, I256 M) { return _mm256_add_epi32(_mm256_sub_epi32(x, y), M); }
I256 add32(I256 x, I256 y, I256 M) { return shrk32(_mm256_add_epi32(x, y), M); }
I256 sub32(I256 x, I256 y, I256 M) { return dilt32(_mm256_sub_epi32(x, y), M); }
I256 reduce(I256 a, I256 b, I256 niv, I256 M) {
    I256 c = _mm256_mul_epu32(a, niv), d = _mm256_mul_epu32(b, niv);
    c = _mm256_mul_epu32(c, M), d = _mm256_mul_epu32(d, M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a, c), 32), _mm256_add_epi64(b, d), 0xaa);
}
I256 mul(I256 a, I256 b, I256 niv, I256 M) {
    return reduce(_mm256_mul_epu32(a, b), _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32)), niv, M);
}
I256 mul_bsm(I256 a, I256 b, I256 niv, I256 M) {
    return reduce(_mm256_mul_epu32(a, b), _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b), niv, M);
}
I256 mul_bsmfxd(I256 a, I256 b, I256 bniv, I256 M) {
    I256 cc = _mm256_mul_epu32(a, bniv), dd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), bniv);
    I256 c = _mm256_mul_epu32(a, b), d = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b);
    cc = _mm256_mul_epu32(cc, M), dd = _mm256_mul_epu32(dd, M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(c, cc), 32), _mm256_add_epi64(d, dd), 0xaa);
}
I256 mul_upd_rt(I256 a, I256 bu, I256 M) {
    I256 cc = _mm256_mul_epu32(a, bu), c = _mm256_mul_epu32(a, _mm256_srli_epi64(bu, 32));
    cc = _mm256_mul_epu32(cc, M);
    return shrk32(_mm256_srli_epi64(_mm256_add_epi64(c, cc), 32), M);
}

constexpr auto _mxlg = 26, _lg_itth = 6;
constexpr auto _itth = usize(1) << _lg_itth;
static_assert(_lg_itth % 2 == 0);

template <typename mint>
struct NTT_Info {
    static constexpr u32 mod = mint::mod();
    static constexpr MontgomeryReduction<u32> mr{mod};
    static constexpr mint pr = primitive_root_constexpr(mod);
    static constexpr int lvl = __builtin_ctz(mod - 1);

    u32 img{}, imgniv{};
    mint RT1[_mxlg]{};
    alignas(32) std::array<u32, 8> rt3[_mxlg - 2]{}, rt3i[_mxlg - 2]{}, bwbr{}, bwb{}, bwbi{}, rt4[_mxlg - 3]{}, rt4niv[_mxlg - 3]{}, rt4i[_mxlg - 3]{}, rt4iniv[_mxlg - 3]{}, pr2{}, pr4{}, pr2niv{}, pr4niv{}, pr2i{}, pr2iniv{}, pr4i{}, pr4iniv{};
    constexpr NTT_Info() {
        struct {
            std::array<mint, 8> rt4;
            std::array<mint, 8> rt4i;
        } tmp{};
        constexpr u32 niv = mr.mod_neg_inv();
        mint w[_mxlg - 1], y[_mxlg - 1];
        w[lvl - 2] = power(pr, (mod - 1) >> lvl), y[lvl - 2] = w[lvl - 2].inv();
        _for_r (i, lvl - 2) {
            w[i] = w[i + 1] * w[i + 1];
            y[i] = y[i + 1] * y[i + 1];
        }
        RT1[lvl - 1] = power(w[lvl - 2], 3);
        _for_r (i, lvl - 1)
            RT1[i] = RT1[i + 1] * RT1[i + 1];
        img = w[0].residue(), imgniv = img * niv;
        assignr(bwbr, 1, 0, 1, 0, 1);
        assignr(bwb, w[1], 0, w[0], 0, -w[0] * w[1]);
        assignr(bwbi, y[1], 0, y[0], 0, y[0] * y[1]);
        mint pr = 1, pri = 1;
        _for (i, lvl - 2) {
            const mint r = pr * w[i + 1], ri = pri * y[i + 1];
            const mint r2 = r * r, r2i = ri * ri;
            const mint r3 = r * r2, r3i = ri * r2i;
            rt3[i] = {r.residue() * niv, r.residue(), r2.residue() * niv, r2.residue(), r3.residue() * niv, r3.residue()};
            rt3i[i] = {ri.residue() * niv, ri.residue(), r2i.residue() * niv, r2i.residue(), r3i.residue() * niv, r3i.residue()};
            pr = pr * y[i + 1], pri = pri * w[i + 1];
        }
        pr = pri = 1;
        _for (i, lvl - 3) {
            const mint r = pr * w[i + 2], ri = pri * y[i + 2];
            tmp.rt4[0] = tmp.rt4i[0] = 1;
            _for (j, 7) {
                tmp.rt4[j + 1] = tmp.rt4[j] * r;
                tmp.rt4i[j + 1] = tmp.rt4i[j] * ri;
            }
            copyr(rt4[i], tmp.rt4);
            copyr(rt4i[i], tmp.rt4i);
            _for (j, 8) {
                rt4niv[i][j] = rt4[i][j] * niv;
                rt4iniv[i][j] = rt4i[i][j] * niv;
            }
            pr = pr * y[i + 2], pri = pri * w[i + 2];
        }
        assignr(pr2, 1, 1, 1, w[0], 1, 1, 1, w[0]);
        assignr(pr4, 1, 1, 1, 1, 1, w[1], w[0], w[0] * w[1]);
        const u32 nr2 = mod - mr.mbase2(), imgr2 = mint(img).residue();
        pr2i = {nr2, nr2, nr2, imgr2, nr2, nr2, nr2, imgr2};
        assignr(pr4i, 1, 1, 1, 1, 1, y[1], y[0], y[0] * y[1]);
        _for (j, 8) {
            pr2niv[j] = pr2[j] * niv, pr4niv[j] = pr4[j] * niv;
            pr2iniv[j] = pr2i[j] * niv, pr4iniv[j] = pr4i[j] * niv;
        }
    }

private:
    static constexpr void copyr(auto&& res, auto&& src) {
        _for (i, len(res))
            res[i] = src[i].residue();
    }
    static constexpr void assignr(auto&& res, auto&&... args) { res = {mint(args).residue()...}; }
};
struct U32Aligned {
    u32* p;
    U32Aligned(usize n) { p = new (std::align_val_t{32}) u32[n]; }
    ~U32Aligned() { ::operator delete[](p, std::align_val_t{32}); }
    U32Aligned(const U32Aligned&) = delete;
    U32Aligned& operator=(const U32Aligned&) = delete;
    operator u32*() const { return (u32*)p; }
};
template <typename mint>
void vec_dif(I256* const f, const usize n) {
    static constexpr NTT_Info<mint> info;
    alignas(32) std::array<u32, 8> st_1[_mxlg >> 1];
    const I256 Mod = _mm256_set1_epi32(info.mod), Mod2 = _mm256_set1_epi32(info.mr.mod2()), Niv = _mm256_set1_epi32(info.mr.mod_neg_inv());
    const I256 Img = _mm256_set1_epi32(info.img), ImgNiv = _mm256_set1_epi32(info.imgniv), id = _mm256_setr_epi32(0, 2, 0, 4, 0, 2, 0, 4);
    const int lgn = __builtin_ctzll(n);
    fill(st_1, st_1 + (lgn >> 1), info.bwb);
    const usize nn = n >> (lgn & 1), m = min(n, _itth), mm = min(nn, _itth);
    if (nn != n) {
        _for (i, nn) {
            const auto p0 = f + i, p1 = f + nn + i;
            const auto f0 = load256(p0), f1 = load256(p1);
            const auto g0 = add32(f0, f1, Mod2), g1 = Lsub32(f0, f1, Mod2);
            store256(p0, g0), store256(p1, g1);
        }
    }
    for (usize L = nn >> 2; L > 0; L >>= 2) {
        _for (i, L) {
            const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
            const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
            const auto g3 = mul_bsmfxd(Lsub32(f1, f3, Mod2), Img, ImgNiv, Mod), g1 = add32(f1, f3, Mod2);
            const auto g0 = add32(f0, f2, Mod2), g2 = sub32(f0, f2, Mod2);
            const auto h0 = add32(g0, g1, Mod2), h1 = Lsub32(g0, g1, Mod2);
            const auto h2 = Ladd32(g2, g3, Mod2), h3 = Lsub32(g2, g3, Mod2);
            store256(p0, h0), store256(p1, h1), store256(p2, h2), store256(p3, h3);
        }
    }
    _for (j, 0, n, m) {
        int t = ((j == 0) ? min(_lg_itth, lgn) : __builtin_ctzll(j)) & -2, p = (t - 2) >> 1;
        for (usize L = (usize(1) << t) >> 2; L >= _itth; L >>= 2, t -= 2, p--) {
            auto rt = load256(st_1 + p);
            const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
            const auto r1Niv = _mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt, Niv), id);
            rt = mul_upd_rt(rt, load256(info.rt3 + __builtin_ctzll(~j >> t)), Mod);
            const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB), nr3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
            const auto r2Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_BBBB), nr3Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_DDDD);
            store256(st_1 + p, rt);
            _for (i, L) {
                const auto p0 = f + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
                const auto g1 = mul_bsmfxd(f1, r1, r1Niv, Mod), ng3 = mul_bsmfxd(f3, nr3, nr3Niv, Mod);
                const auto g2 = mul_bsmfxd(f2, r2, r2Niv, Mod), g0 = shrk32(f0, Mod2);
                const auto h3 = mul_bsmfxd(Ladd32(g1, ng3, Mod2), Img, ImgNiv, Mod), h1 = sub32(g1, ng3, Mod2);
                const auto h0 = add32(g0, g2, Mod2), h2 = sub32(g0, g2, Mod2);
                const auto u0 = Ladd32(h0, h1, Mod2), u1 = Lsub32(h0, h1, Mod2);
                const auto u2 = Ladd32(h2, h3, Mod2), u3 = Lsub32(h2, h3, Mod2);
                store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
            }
        }
        I256* const g = f + j;
        for (usize l = mm, L = mm >> 2; L; l = L, L >>= 2, t -= 2, p--) {
            auto rt = load256(st_1 + p);
            for (usize i = (j == 0 ? l : 0), k = (j + i) >> t; i < m; i += l, k++) {
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB);
                const auto nr3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                _for (j, L) {
                    const auto p0 = g + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
                    const auto g1 = mul_bsm(f1, r1, Niv, Mod), ng3 = mul_bsm(f3, nr3, Niv, Mod);
                    const auto g2 = mul_bsm(f2, r2, Niv, Mod), g0 = shrk32(f0, Mod2);
                    const auto h3 = mul_bsmfxd(Ladd32(g1, ng3, Mod2), Img, ImgNiv, Mod), h1 = sub32(g1, ng3, Mod2);
                    const auto h0 = add32(g0, g2, Mod2), h2 = sub32(g0, g2, Mod2);
                    const auto u0 = Ladd32(h0, h1, Mod2), u1 = Lsub32(h0, h1, Mod2);
                    const auto u2 = Ladd32(h2, h3, Mod2), u3 = Lsub32(h2, h3, Mod2);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
                rt = mul_upd_rt(rt, load256(info.rt3 + __builtin_ctzll(~k)), Mod);
            }
            store256(st_1 + p, rt);
        }
    }
}
template <typename mint, bool shrk = false>
void vec_dit(I256* const f, usize n) {
    static constexpr NTT_Info<mint> info;
    alignas(32) std::array<u32, 8> st_1[_mxlg >> 1];
    const I256 Mod = _mm256_set1_epi32(info.mod), Mod2 = _mm256_set1_epi32(info.mr.mod2()), Niv = _mm256_set1_epi32(info.mr.mod_neg_inv());
    const I256 Img = _mm256_set1_epi32(info.img), ImgNiv = _mm256_set1_epi32(info.imgniv), id = _mm256_setr_epi32(0, 2, 0, 4, 0, 2, 0, 4);
    const int lgn = __builtin_ctzll(n);
    fill(st_1, st_1 + (_lg_itth >> 1), info.bwbr);
    fill(st_1 + (_lg_itth >> 1), st_1 + (_mxlg >> 1), info.bwbi);
    const usize nn = n >> (lgn & 1), mm = min(nn, _itth);
    _for (j, 0, n, mm) {
        I256* const g = f + j;
        int t = 2, p = 0;
        for (usize l = 4, L = 1; l <= mm; L = l, l <<= 2, t += 2, p++) {
            auto rt = load256(st_1 + p);
            for (usize i = 0, k = j >> t; i < mm; i += l, k++) {
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB);
                const auto r3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                _for (j, L) {
                    const auto p0 = g + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f0 = load256(p0), f1 = load256(p1), f2 = load256(p2), f3 = load256(p3);
                    const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                    const auto g2 = add32(f2, f3, Mod2), g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod);
                    const auto h0 = Ladd32(g0, g2, Mod2), h1 = Ladd32(g1, g3, Mod2);
                    const auto h2 = Lsub32(g0, g2, Mod2), h3 = Lsub32(g1, g3, Mod2);
                    const auto u0 = shrk32(h0, Mod2), u1 = mul_bsm(h1, r1, Niv, Mod);
                    const auto u2 = mul_bsm(h2, r2, Niv, Mod), u3 = mul_bsm(h3, r3, Niv, Mod);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
                rt = mul_upd_rt(rt, load256(info.rt3i + __builtin_ctzll(~k)), Mod);
            }
            store256(st_1 + p, rt);
        }
        int tt = min(__builtin_ctzll(~(j >> _lg_itth)) + _lg_itth, lgn);
        for (usize L = _itth, l = L << 2; t <= tt; L = l, l <<= 2, t += 2, p++) {
            if ((j + _itth) == l) {
                if (shrk && l == n) {
                    _for (i, L) {
                        const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                        const auto f2 = load256(p2), f3 = load256(p3), f0 = load256(p0), f1 = load256(p1);
                        const auto g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod), g2 = add32(f2, f3, Mod2);
                        const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                        const auto h0 = add32(g0, g2, Mod2), h1 = add32(g1, g3, Mod2);
                        const auto h2 = sub32(g0, g2, Mod2), h3 = sub32(g1, g3, Mod2);
                        const auto u0 = shrk32(h0, Mod), u1 = shrk32(h1, Mod);
                        const auto u2 = shrk32(h2, Mod), u3 = shrk32(h3, Mod);
                        store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                    }
                }
                else {
                    _for (i, L) {
                        const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                        const auto f2 = load256(p2), f3 = load256(p3), f0 = load256(p0), f1 = load256(p1);
                        const auto g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod), g2 = add32(f2, f3, Mod2);
                        const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                        const auto h0 = add32(g0, g2, Mod2), h1 = add32(g1, g3, Mod2);
                        const auto h2 = sub32(g0, g2, Mod2), h3 = sub32(g1, g3, Mod2);
                        store256(p0, h0), store256(p1, h1), store256(p2, h2), store256(p3, h3);
                    }
                }
            }
            else {
                auto rt = load256(st_1 + p);
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r1Niv = _mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt, Niv), id);
                rt = mul_upd_rt(rt, load256(info.rt3i + __builtin_ctzll(~j >> t)), Mod);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB), r3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                const auto r2Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_BBBB), r3Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_DDDD);
                store256(st_1 + p, rt);
                _for (i, L) {
                    const auto p0 = f + j + _itth - l + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f0 = load256(p0), f1 = load256(p1), f2 = load256(p2), f3 = load256(p3);
                    const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                    const auto g2 = add32(f2, f3, Mod2), g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod);
                    const auto h0 = Ladd32(g0, g2, Mod2), h1 = Ladd32(g1, g3, Mod2);
                    const auto h2 = Lsub32(g0, g2, Mod2), h3 = Lsub32(g1, g3, Mod2);
                    const auto u0 = shrk32(h0, Mod2), u1 = mul_bsmfxd(h1, r1, r1Niv, Mod);
                    const auto u2 = mul_bsmfxd(h2, r2, r2Niv, Mod), u3 = mul_bsmfxd(h3, r3, r3Niv, Mod);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
            }
        }
    }
    if (shrk && nn == n && n <= _itth) {
        _for (i, n) {
            const auto f0 = load256(f + i);
            store256(f + i, shrk32(f0, Mod));
        }
    }
    if (nn != n) {
        _for (i, nn) {
            const auto p0 = f + i, p1 = f + nn + i;
            const auto f0 = load256(p0), f1 = load256(p1);
            const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
            if constexpr (shrk) {
                const auto h0 = shrk32(g0, Mod), h1 = shrk32(g1, Mod);
                store256(p0, h0), store256(p1, h1);
            }
            else {
                store256(p0, g0), store256(p1, g1);
            }
        }
    }
}
//f[0,8) = fx * f[0,8) * g[0,8) (mod x^8 - ww)
[[gnu::always_inline]] inline void conv8(I256* f, const I256* g, I256 ww, I256 fx, I256 Niv, I256 Mod, I256 Mod2) {
    const auto raa = load256(f), rbb = load256(g);
    const auto taa = shrk32(raa, Mod2), bb = shrk32(mul_bsm(rbb, fx, Niv, Mod), Mod);
    const auto aw = shrk32(mul_bsm(taa, ww, Niv, Mod), Mod);
    const auto aa = shrk32(taa, Mod);
    const auto awa = _mm256_permute2x128_si256(aa, aw, 3);
    const auto b0 = _mm256_permute4x64_epi64(bb, 0x00), b1 = _mm256_shuffle_epi32(b0, _MM_PERM_CDAB);
    const auto a0 = aa, a1 = _mm256_srli_epi64(a0, 32);
    const auto aw7 = _mm256_alignr_epi8(aa, awa, 12);
    auto res00 = _mm256_mul_epu32(a0, b0);
    auto res01 = _mm256_mul_epu32(a1, b0);
    auto res10 = _mm256_mul_epu32(aw7, b1);
    auto res11 = _mm256_mul_epu32(a0, b1);
    const auto b2 = _mm256_permute4x64_epi64(bb, 0x55), b3 = _mm256_shuffle_epi32(b2, _MM_PERM_CDAB);
    const auto aw6 = _mm256_alignr_epi8(aa, awa, 8);
    const auto aw5 = _mm256_alignr_epi8(aa, awa, 4);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(aw6, b2));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw7, b2));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw5, b3));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(aw6, b3));
    const auto b4 = _mm256_permute4x64_epi64(bb, 0xaa), b5 = _mm256_shuffle_epi32(b4, _MM_PERM_CDAB);
    const auto aw3 = _mm256_alignr_epi8(awa, aw, 12);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(awa, b4));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw5, b4));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw3, b5));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(awa, b5));
    const auto b6 = _mm256_permute4x64_epi64(bb, 0xff), b7 = _mm256_shuffle_epi32(b6, _MM_PERM_CDAB);
    const auto aw2 = _mm256_alignr_epi8(awa, aw, 8);
    const auto aw1 = _mm256_alignr_epi8(awa, aw, 4);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(aw2, b6));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw3, b6));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw1, b7));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(aw2, b7));
    res00 = _mm256_add_epi64(res00, res10);
    res01 = _mm256_add_epi64(res01, res11);
    store256(f, shrk32(reduce(res00, res01, Niv, Mod), Mod2));
}
template <typename mint>
void vec_cvdt(I256* f, const I256* g, usize sz) {
    static constexpr NTT_Info<mint> info;
    const auto mod = info.mod, niv = info.mr.mod_neg_inv();
    const auto Fx = _mm256_set1_epi32(mint::from_int(mint(sz).inv().residue()).residue());
    const auto Niv = _mm256_set1_epi32(niv), Mod = _mm256_set1_epi32(mod), Mod2 = _mm256_set1_epi32(info.mr.mod2());
    mint rr = 1;
    _for (i, sz) {
        conv8(f + i, g + i, _mm256_set1_epi32(rr.raw()), Fx, Niv, Mod, Mod2);
        rr *= info.RT1[__builtin_ctzll(~i)];
    }
}
template <typename mint>
void conv(u32* f, u32* g, usize sz) {
    I256 *F = (I256*)f, *G = (I256*)g;
    vec_dif<mint>(F, sz >> 3);
    vec_dif<mint>(G, sz >> 3);
    vec_cvdt<mint>(F, G, sz >> 3);
    vec_dit<mint, true>(F, sz >> 3);
}
template <typename mint>
void conv(u32* f, usize sz) {
    I256* F = (I256*)f;
    vec_dif<mint>(F, sz >> 3);
    vec_cvdt<mint>(F, F, sz >> 3);
    vec_dit<mint, true>(F, sz >> 3);
}