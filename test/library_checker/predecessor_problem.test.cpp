#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include "all.hpp"
#include "ds/fastset.hpp"

int main() {
    dR(u32, n, q);
    STR(s, n);
    FastSet st(n);
    _for (i, n)
        if (s[i] == '1')
            st.insert(i);
    _for (q) {
        dR(u32, t, x);
        if (t == 0)
            st.insert(x);
        else if (t == 1)
            st.erase(x);
        else if (t == 2)
            print(st.contains(x));
        else if (t == 4)
            print(st.prev(x));
        else {
            u32 r = st.next(x);
            if (r == n)
                print(-1);
            else
                print(r);
        }
    }
    return 0;
}