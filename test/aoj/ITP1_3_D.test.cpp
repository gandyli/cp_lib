#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_3_D"

#include "all.hpp"
#include "math/divisors.hpp"

int main() {
    dR(int, a, b, c);
    int ans = 0;
    foreach(d, divisors(u32(c)))
        if (a <= d && d <= b)
            ans++;
    print(ans);
    return 0;
}