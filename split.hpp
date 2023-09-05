#pragma once
#include "template.hpp"

Vec<str> split(const str& s, char sep = ' ') {
    Vec<str> ans{{}};
    foreach(c, s)
        if (c == sep)
            ans.eb();
        else
            ans.back().push_back(c);
    return ans;
}

Vec<str> split(const str& s, str seps = " ,") {
    Vec<str> ans{{}};
    foreach(c, s)
        if (count(seps, c))
            ans.eb();
        else
            ans.back().push_back(c);
    return ans;
}