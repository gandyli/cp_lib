#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_9_A"

#include "all.hpp"
#include "string/split.hpp"

int main() {
    dR(str, t);
    int ans = 0;
    str line;
    while (io.readline(line) && line != "END_OF_TEXT") {
        foreach (s, split(line, ' ')) {
            foreach (c, s)
                c = tolower(c);
            if (s == t)
                ans++;
        }
    }
    writeln(ans);
    return 0;
}