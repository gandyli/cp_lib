#pragma once
#include "template.hpp"

#if defined(__unix__)
#include <sys/mman.h>
#include <sys/stat.h>

struct MmapReader {
    static constexpr int ev = 0;

    char* ip;
    MmapReader(FILE* f) {
        struct stat st;
        int fd;
        if (f)
            fd = fileno(f), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    }
    MmapReader(std::string_view s): MmapReader(fopen(s.data(), "r")) {}
    int getch() { return *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() const { return *ip; }
    void ireadstr(char* s, usize n) {
        memcpy(s, ip, n);
        ip += n;
    }
    usize next_size(auto&& f) const {
        char* p = ip;
        while (!f(*p))
            p++;
        return p - ip;
    }
    void skipws() {
        while (*ip <= ' ')
            ip++;
    }
    void read(char* s) {
        skipws();
        auto n = next_size([](char c) { return c <= ' '; });
        ireadstr(s, n);
        s[n] = 0;
    }
    void read(str& s) {
        skipws();
        auto n = next_size([](char c) { return c <= ' '; });
        s.assign(ip, n);
        ip += n;
    }
    void readline(char* s) {
        auto n = next_size([](char c) { return c == '\n' || c == ev; });
        ireadstr(s, n), ip++;
        s[n] = 0;
    }
    void readline(str& s) {
        auto n = next_size([](char c) { return c == '\n' || c == ev; });
        s.assign(ip, n);
        ip += n + 1;
    }
    void readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
    }
    void readstr(str& s, usize n) {
        skipws();
        s.assign(ip, n);
        ip += n;
    }
    char*& raw_ip() { return ip; }
    template <int>
    void ensure() {}
};
#endif