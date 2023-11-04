#pragma once
#include "template.hpp"

vi restore_path(vi par, int t) {
    vi r{t};
    while (par[r.back()] != -1)
        r.eb(par[r.back()]);
    reverse(r);
    return r;
}