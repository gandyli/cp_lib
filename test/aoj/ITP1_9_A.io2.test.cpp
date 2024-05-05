#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_9_A"

#include "all2.hpp"
#include "string/split.hpp"

int main() {
    dR(str, t);
    int ans = 0;
    str line;
    while (io.readline(line)) {
        foreach (s, split(line)) {
            transform(s, s.begin(), ::tolower);
            if (s == t)
                ans++;
        }
    }
    print(ans);
    return 0;
}