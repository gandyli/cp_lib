#pragma once
#include "../../template.hpp"

auto prime_sieve(int n, int Q = 17, int L = 1 << 15) {
    static constexpr int rs[]{1, 7, 11, 13, 17, 19, 23, 29};
    struct P {
        P(int p): p(p) {}
        int p;
        int pos[8];
    };
    auto approx_prime_count = [](int n) -> int { return n > 60184 ? n / (log(n) - 1.1) : max(1., n / (log(n) - 1.11)) + 1; };

    const int v = int(sqrt(n)) + 1, vv = int(sqrt(sqrt(n))) + 1;
    Vec<bool> isp(v, true);
    _for (i, 2, vv)
        if (isp[i])
            _for (j, i * i, v, i)
                isp[j] = false;

    const int rsize = approx_prime_count(n + 30);
    vi primes{2, 3, 5};
    int psize = 3;
    primes.resize(rsize);

    Vec<P> sprimes;
    usize pbeg = 0;
    int prod = 1;
    _for (p, 7, v)
        if (isp[p]) {
            if (p <= Q)
                prod *= p, pbeg++, primes[psize++] = p;
            P pp(p);
            _for (t, 8) {
                int j = (p <= Q) ? p : p * p;
                while (j % 30 != rs[t])
                    j += p << 1;
                pp.pos[t] = j / 30;
            }
            sprimes.push_back(pp);
        }

    Vec<u8> pre(prod, 0xff);
    _for (pi, pbeg) {
        auto pp = sprimes[pi];
        const int p = pp.p;
        _for (t, 8) {
            const u8 m = ~(1 << t);
            _for (i, pp.pos[t], prod, p)
                pre[i] &= m;
        }
    }

    const int block_size = ceil(L, prod) * prod;
    Vec<u8> block(block_size);
    u8* pblock = block.data();
    const int M = ceil(n, 30);
    for (int beg = 0; beg < M; beg += block_size, pblock -= block_size) {
        int end = min(M, beg + block_size);
        _for (i, beg, end, prod)
            copy(pre, pblock + i);
        if (beg == 0)
            pblock[0] &= 0xfe;
        _for (pi, pbeg, sprimes.size()) {
            auto& pp = sprimes[pi];
            const int p = pp.p;
            _for (t, 8) {
                int i = pp.pos[t];
                const u8 m = ~(1 << t);
                for (; i < end; i += p)
                    pblock[i] &= m;
                pp.pos[t] = i;
            }
        }
        _for (i, beg, end)
            for (int m = pblock[i]; m; m &= m - 1)
                primes[psize++] = i * 30 + rs[__builtin_ctz(m)];
    }
    assert(psize <= rsize);
    while (psize && primes[psize - 1] > n)
        psize--;
    primes.resize(psize);
    return primes;
}

auto prime_table(int N) {
    Vec<bool> t(N + 1);
    for (auto p: prime_sieve(N))
        t[p] = true;
    return t;
}