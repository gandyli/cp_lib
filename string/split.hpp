#pragma once
#include "template.hpp"

vc<str> split(const str& s, char sep = ' ') {
    vc<str> ans{{}};
    foreach (c, s)
        if (c == sep)
            ans.eb();
        else
            ans.back().push_back(c);
    return ans;
}

vc<str> split(const str& s, str seps = " ,") {
    vc<str> ans{{}};
    foreach (c, s)
        if (count(seps, c))
            ans.eb();
        else
            ans.back().push_back(c);
    return ans;
}