#pragma once
#include "template.hpp"

std::vector<int> prime_sieve(int n, int Q = 17, int L = 1 << 15) {
    static constexpr int rs[]{1, 7, 11, 13, 17, 19, 23, 29};
    struct P {
        P(int p): p(p) {}
        int p;
        int pos[8];
    };
    auto approx_prime_count = [](int n) -> int { return n > 60184 ? n / (log(n) - 1.1) : std::max(1., n / (log(n) - 1.11)) + 1; };

    const int v = sqrt(n), vv = sqrt(v);
    std::vector<bool> isp(v + 1, true);
    for (int i = 2; i <= vv; i++)
        if (isp[i])
            for (int j = i * i; j <= v; j += i)
                isp[j] = false;

    const int rsize = approx_prime_count(n + 30);
    std::vector<int> primes{2, 3, 5};
    int psize = 3;
    primes.resize(rsize);

    std::vector<P> sprimes;
    size_t pbeg = 0;
    int prod = 1;
    for (int p = 7; p <= v; p++)
        if (isp[p]) {
            if (p <= Q)
                prod *= p, pbeg++, primes[psize++] = p;
            P pp(p);
            for (int t = 0; t < 8; t++) {
                int j = (p <= Q) ? p : p * p;
                while (j % 30 != rs[t])
                    j += p << 1;
                pp.pos[t] = j / 30;
            }
            sprimes.push_back(pp);
        }

    std::vector<u8> pre(prod, 0xff);
    for (size_t pi = 0; pi < pbeg; pi++) {
        auto pp = sprimes[pi];
        const int p = pp.p;
        for (int t = 0; t < 8; t++) {
            const u8 m = ~(1 << t);
            for (int i = pp.pos[t]; i < prod; i += p)
                pre[i] &= m;
        }
    }

    const int block_size = ceil(L, prod) * prod;
    std::vector<u8> block(block_size);
    u8* pblock = block.data();
    const int M = ceil(n, 30);
    for (int beg = 0; beg < M; beg += block_size, pblock -= block_size) {
        int end = std::min(M, beg + block_size);
        for (int i = beg; i < end; i += prod)
            std::copy(pre.begin(), pre.end(), pblock + i);
        if (beg == 0)
            pblock[0] &= 0xfe;
        for (size_t pi = pbeg; pi < sprimes.size(); pi++) {
            auto& pp = sprimes[pi];
            const int p = pp.p;
            for (int t = 0; t < 8; t++) {
                int i = pp.pos[t];
                const u8 m = ~(1 << t);
                for (; i < end; i += p)
                    pblock[i] &= m;
                pp.pos[t] = i;
            }
        }
        for (int i = beg; i < end; i++)
            for (int m = pblock[i]; m > 0; m &= m - 1)
                primes[psize++] = i * 30 + rs[__builtin_ctz(m)];
    }
    assert(psize <= rsize);
    while (psize && primes[psize - 1] > n)
        psize--;
    primes.resize(psize);
    return primes;
}

std::vector<int> prime_table(int N) {
    std::vector<int> t(N + 1);
    for (auto p: prime_sieve(N))
        t[p] = 1;
    return t;
}