#pragma once
#include "template.hpp"

#define FASTIO
#ifdef CHECK_EOF
#define ECHK0 *ptr ? *ptr++ : 0
#define ECHK1     \
    if (ch == EV) \
        return set(false), *this;
#define ECHK2  \
    if (!*ptr) \
        return set(false), *this;
#define ECHK3 &&ch != -1
#define ECHK4 &&*ptr
#define ECHK5     \
    if (ch == -1) \
        return set(false);
#define ECHK6  \
    if (!*ptr) \
        return set(false);
#else
#define ECHK0 *ptr++
#define ECHK1
#define ECHK2
#define ECHK3
#define ECHK4
#define ECHK5
#define ECHK6
#endif

#if defined(__unix__) && !defined(LX_DEBUG) && !defined(LX_LOCAL)
#define USE_MMAP
#define EV 0
#include <sys/mman.h>
#include <sys/stat.h>
#else
#define EV (-1)
#endif

constexpr usize bufSize = 1 << 20;

class In {
    friend class IO;

private:
    FILE* inFile;
    bool status = true;
#ifdef USE_MMAP
    struct stat st;
    char* ptr;
    int fd;
#elif !defined(LX_DEBUG)
    char buf[bufSize], *p1, *p2;
#endif
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }
    static constexpr bool blank(int c) { return c <= ' '; }

public:
#ifdef LX_DEBUG
    int getch() { return fgetc(inFile); }
    int getch_unchecked() { return getch(); }
    int unget(int c = 0) { return ungetc(c, inFile); }
    int peek() { return unget(getch()); }
    void input(FILE* f) { inFile = f, set(); }
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        unget(ch);
    }
    void readstr(char* s, usize n) { fread(s, 1, n, inFile); }
#elif defined(USE_MMAP)
    void skipws() {
        while (blank(*ptr) ECHK4)
            ptr++;
        ECHK6
    }
    int unget(int = 0) = delete;
    int getch() { return ECHK0; }
    int getch_unchecked() { return *ptr++; }
    int peek() { return *ptr; }
    void input(FILE* f) {
        inFile = f;
        if (inFile)
            fd = fileno(inFile), fstat(fd, &st), ptr = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0), set();
    }
    void readstr(char* s, usize n) { memcpy(s, ptr, n), ptr += n; }
    static constexpr auto n = [] {
        std::array<u32, 0x10000> n{};
        fill(n, -1);
        _for (i, 10)
            _for (j, 10)
                n[i + j * 0x100 + 0x3030] = i * 10 + j;
        return n;
    }();
#else
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        ECHK5
        unget();
    }
    int unget(int = 0) { return *p1--; }
    int getch() { return (p1 == p2 ? (p2 = (p1 = buf) + fread(buf, 1, bufSize, inFile)) : nullptr), p1 == p2 ? -1 : *p1++; }
    int getch_unchecked() { return *p1++; }
    int peek() { return (p1 == p2 ? (p2 = (p1 = buf) + fread(buf, 1, bufSize, inFile)) : nullptr), p1 == p2 ? -1 : *p1; }
    void input(FILE* f) { inFile = f, p1 = p2 = buf, set(); }
    void readstr(char* s, usize n) {
        if (usize len = p2 - p1; n > len) [[unlikely]] {
            memcpy(s, p1, len);
            n -= len;
            s += len;
            p1 = p2;
            fread(s, 1, n, inFile);
        }
        else
            memcpy(s, p1, n), p1 += n;
    }
#endif
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool s = true) { status = s; }
};
class Out {
    friend class IO;

private:
    FILE* outFile;
#ifndef LX_DEBUG
    char pbuf[bufSize], *pp = pbuf;
#endif

public:
#ifdef LX_DEBUG
    void flush() { fflush(outFile); }
    void putch_unchecked(char c) { fputc(c, outFile); }
    void putch(char c) { putch_unchecked(c); }
    void writestr(const char* s, usize n) { fwrite(s, 1, n, outFile); }
#else
    void flush() { fwrite(pbuf, 1, pp - pbuf, outFile), pp = pbuf; }
    void putch_unchecked(char c) { *pp++ = c; }
    void putch(char c) { (pp == end(pbuf) ? flush() : void()), putch_unchecked(c); }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(pbuf) - pp)) [[unlikely]]
            flush(), fwrite(s, 1, n, outFile);
        else
            memcpy(pp, s, n), pp += n;
    }
    static constexpr auto D = [] {
        std::array<u32, 10000> m{};
        int x = 0;
        _for (i, 10)
            _for (j, 10)
                _for (k, 10)
                    _for (l, 10)
                        m[x++] = i + j * 0x100 + k * 0x10000 + l * 0x1000000 + 0x30303030;
        return m;
    }();
#endif
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { outFile = f; }
};
class IO: public In, public Out {
private:
    u32 precision = 12;

public:
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
#ifdef LX_DEBUG
    void flush() { Out::flush(), set(); }
#endif
    template <typename... Args>
        requires (sizeof...(Args) > 1)
    IO& read(Args&... x) {
#ifdef CHECK_EOF
        (read(x) && ...);
#else
        (read(x), ...);
#endif
        return *this;
    }
    template <Signed T>
    IO& read(T& x) {
        x = 0;
        static make_unsigned_t<T> t;
        bool sign = false;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            sign = ch == '-', ch = getch();
        ECHK1
        t = 0;
        while (isdigit(ch))
            t = t * 10 + (ch ^ 48), ch = getch();
        unget(ch);
#else
        while (!isdigit(*ptr) ECHK4)
            sign = *ptr++ == '-';
        ECHK2
        t = *ptr++ ^ 48;
        while (~n[*reinterpret_cast<u16*&>(ptr)])
            t = t * 100 + n[*reinterpret_cast<u16*&>(ptr)++];
        if (isdigit(*ptr))
            t = t * 10 + (*ptr++ ^ 48);
#endif
        x = sign ? -t : t;
        return *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            ch = getch();
        ECHK1
        while (isdigit(ch))
            x = x * 10 + (ch ^ 48), ch = getch();
        unget(ch);
#else
        while (!isdigit(*ptr) ECHK4)
            ptr++;
        ECHK2
        x = *ptr++ ^ 48;
        while (~n[*reinterpret_cast<u16*&>(ptr)])
            x = x * 100 + n[*reinterpret_cast<u16*&>(ptr)++];
        if (isdigit(*ptr))
            x = x * 10 + (*ptr++ ^ 48);
#endif
        return *this;
    }
    IO& read(std::floating_point auto& x) {
        static str s;
        if (read(s))
            std::from_chars(s.begin().base(), s.end().base(), x);
        return *this;
    }
    template <typename T = int>
    T read() {
        static std::decay_t<T> x;
        return read(x), x;
    }
    IO& read(char& ch) {
        skipws();
        ch = getch();
        ECHK1
        return *this;
    }
#ifdef USE_MMAP
    usize next_size() const {
        char* ptr = this->ptr;
        while (!blank(*ptr) ECHK4)
            ptr++;
        return ptr - this->ptr;
    }
    IO& read(char* s) {
        skipws();
        auto n = next_size();
        In::readstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        auto n = next_size();
        s.assign(ptr, n);
        ptr += n;
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.assign(ptr, n);
        ptr += n;
        return *this;
    }
#else
    IO& read(char* s) {
        skipws();
        int ch = peek();
        ECHK1
        while (!blank(ch))
            *s++ = ch, getch_unchecked(), ch = peek();
        *s = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        int ch = peek();
        ECHK1
        s.erase();
        while (!blank(ch))
            s.append(1, ch), getch_unchecked(), ch = peek();
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        In::readstr(s.data(), n);
        return *this;
    }
#endif
    IO& readstr(char* s, usize n) {
        skipws();
        In::readstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& readline(char* s) {
        char* t = s;
        int ch = getch();
        while (ch != '\n' && ch != EV)
            *s++ = ch, ch = getch();
        *s = 0;
        if (s == t && ch == EV)
            set(false);
        return *this;
    }
    IO& readline(str& s) {
        s.erase();
        int ch = getch();
        while (ch != '\n' && ch != EV)
            s.append(1, ch), ch = getch();
        if (s.empty() && ch == EV)
            set(false);
        return *this;
    }
    IO& read(tupleLike auto& t) {
        return std::apply([&](auto&... t) { (read(t), ...); }, t), *this;
    }
    IO& read(forward_range auto&& r) { return readArray(FORWARD(r)); }
    template <typename T>
        requires requires (T t, IO& io) { t.read(io); }
    IO& read(T& t) { return t.read(*this), *this; }
    void setprecision(u32 n = 6) { precision = n; }
    template <typename... Args>
        requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() const {}
    template <Signed T>
    void write(T x) {
        static make_unsigned_t<T> y;
        y = x;
        if (x < 0)
            putch('-'), write(y = -y);
        else
            write(y);
    }
    void write(std::unsigned_integral auto x) {
#ifndef LX_DEBUG
        if (end(pbuf) - pp < 64) [[unlikely]]
            flush();

        auto L = [&](int x) { return x == 1 ? 0 : ten(x - 1); };
        auto R = [&](int x) { return ten(x) - 1; };

#define de(t)                          \
    case L(t)... R(t):                 \
        *(u32*)pp = D[x / ten((t)-4)]; \
        pp += 4;                       \
        x %= ten((t)-4);               \
        [[fallthrough]]

        u64 y = x;
        switch (y) {
            de(18);
            de(14);
            de(10);
            de(6);
        case L(2)... R(2):
            *(u32*)pp = D[x * 100];
            pp += 2;
            break;

            de(17);
            de(13);
            de(9);
            de(5);
        case L(1)... R(1):
            *pp++ = x ^ 48;
            break;

        default:
            *(u32*)pp = D[x / ten(16)];
            pp += 4;
            x %= ten(16);
            [[fallthrough]];
            de(16);
            de(12);
            de(8);
        case L(4)... R(4):
            *(u32*)pp = D[x];
            pp += 4;
            break;

            de(19);
            de(15);
            de(11);
            de(7);
        case L(3)... R(3):
            *(u32*)pp = D[x * 10];
            pp += 3;
            break;
        }
#undef de
#else
        write(u128(x));
#endif
    }
    void write(u128 x) {
#ifndef LX_DEBUG
        if (end(pbuf) - pp < 64) [[unlikely]]
            flush();
#endif
        static int s[40], t = 0;
        do
            s[t++] = x % 10, x /= 10;
        while (x);
        while (t)
            putch_unchecked(s[--t] ^ 48);
    }
    void write(char c) { putch(c); }
    void write(std::floating_point auto x) {
        static char buf[512];
        writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, precision).ptr - buf);
    }
    void write(std::string_view s) { writestr(s.data(), s.size()); }
    void print_range(auto f, auto l, char d = ' ') {
        if (f != l)
            for (write(*f++); f != l; write(*f++))
                putch(d);
    }
    template <tupleLike T>
    void write(T&& t) {
        [&]<auto... I>(std::index_sequence<I...>) {
            (..., (!I ? void() : putch(' '), write(std::get<I>(t))));
        }(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>());
    }
    template <input_range R>
        requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
        requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)...), print(); }
    void print() { putch('\n'); }
    void print(auto&& x, auto&&... y) {
        write(FORWARD(x));
        ((putch(' '), write(FORWARD(y))), ...);
        print();
    }
    template <std::forward_iterator I>
    IO& readArray(I f, I l) {
        while (f != l)
            read(*f++);
        return *this;
    }
    IO& readArray(forward_range auto&& r) { return readArray(all(r)); }
    template <std::input_iterator I>
    void displayArray(I f, I l, char d = ' ') { print_range(f, l, d), print(); }
    void displayArray(input_range auto&& r, char d = ' ') { displayArray(all(r), d); }
    operator bool() const { return status; }
} io;
#ifdef LX_LOCAL
IO err(nullptr, stderr);
#define dbg(x) err.write(#x " = "), err.writeln(x)
#else
#define dbg(x) \
    do {       \
    } while (false)
#endif
#define dR(type, ...) \
    type __VA_ARGS__; \
    io.read(__VA_ARGS__)
#define dRV(type, a, ...)      \
    VEC(type, a, __VA_ARGS__); \
    io.read(a)
#define STR(s, n) \
    str s;        \
    io.readstr(s, n)
void multipleTests(auto&& f) {
    dR(u32, q);
    _for (q)
        f();
}
void writeln(auto&&... x) { io.writeln(FORWARD(x)...); }
void print(auto&&... x) { io.print(FORWARD(x)...); }
void YES(bool v = true) { io.write(v ? "YES\n" : "NO\n"); }
void NO(bool v = true) { YES(!v); }
void Yes(bool v = true) { io.write(v ? "Yes\n" : "No\n"); }
void No(bool v = true) { Yes(!v); }