#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

#include "all.hpp"
#include "ds/hashmap.hpp"

int main() {
    dR(u32, q);
    HashMap<u64, u64> mp;
    _for (q) {
        dR(u32, t);
        if (t == 0) {
            dR(u64, k, v);
            mp[k] = v;
        }
        else {
            dR(u64, k);
            print(mp.get(k, 0));
        }
    }
    return 0;
}