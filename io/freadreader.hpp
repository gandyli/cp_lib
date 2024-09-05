#pragma once
#include "template.hpp"

struct FreadReader {
    static constexpr int ev = EOF;
    static constexpr usize bufSize = 1 << 20;

    FILE* in;
    char ibuf[bufSize + 8], *ip = ibuf, *eip = ibuf;
    bool eoi = false;
    FreadReader(FILE* f): in(f) {}
    FreadReader(std::string_view s): FreadReader(fopen(s.data(), "r")) {}
    void load() {
        if (eoi) [[unlikely]]
            return;
        usize sz = eip - ip;
        memcpy(ibuf, ip, sz);
        eip = ibuf + sz + fread(ibuf + sz, 1, bufSize - sz, in);
        if (eip != ibuf + bufSize) [[unlikely]]
            eoi = true;
        ip = ibuf;
    }
    int getch() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip; }
    void ireadstr(char* s, usize n) {
        if (usize len = eip - ip; n > len) [[unlikely]] {
            memcpy(s, ip, len);
            n -= len;
            s += len;
            ip = eip;
            fread(s, 1, n, in);
        }
        else
            memcpy(s, ip, n), ip += n;
    }
    void skipws() {
        while (peek() <= ' ')
            ip++;
    }
    void rd(char* s) {
        skipws();
        int ch = peek();
        while (ch > ' ')
            *s++ = ch, ip++, ch = peek();
        *s = 0;
    }
    void rd(str& s) {
        skipws();
        s.clear();
        int ch = peek();
        while (ch > ' ')
            s.push_back(ch), ip++, ch = peek();
    }
    void readline(char* s) {
        int ch = peek();
        while (ch != '\n' && ch != this->ev)
            *s++ = ch, ip++, ch = peek();
        *s = 0;
    }
    void readline(str& s) {
        s.clear();
        int ch = peek();
        while (ch != '\n' && ch != this->ev)
            s.push_back(ch), ip++, ch = peek();
    }
    void readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
    }
    void readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        ireadstr(s.data(), n);
    }
    char*& raw_ip() { return ip; }
    template <int N>
    void ensure() {
        if (eip - ip < N) [[unlikely]]
            load();
    }
};