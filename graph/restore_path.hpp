#pragma once
#include "template.hpp"

vi restore_path(vi par, int t) {
    vi ret{t};
    while (par[ret.back()] != -1)
        ret.eb(par[ret.back()]);
    reverse(ret);
    return ret;
}