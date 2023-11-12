#pragma once
#include "template.hpp"

#define FASTIO
#ifdef CHECK_EOF
#define ECHK0 *ip ? *ip++ : 0
#define ECHK1     \
    if (ch == EV) \
        return set(false), *this;
#define ECHK2 \
    if (!*ip) \
        return set(false), *this;
#define ECHK3 &&ch != -1
#define ECHK4 &&*ip
#define ECHK5     \
    if (ch == -1) \
        return set(false);
#define ECHK6 \
    if (!*ip) \
        return set(false);
#else
#define ECHK0 *ip++
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

class IO {
    static constexpr usize bufSize = 1 << 20;
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }
    static constexpr bool blank(int c) { return c <= ' '; }

    u32 prec = 12;
    FILE *in, *out;
    bool status;
#ifndef LX_DEBUG
    char obuf[bufSize], *ip, *op = obuf;
#ifndef USE_MMAP
    char ibuf[bufSize], *eip;
#endif
#endif

public:
#ifdef LX_DEBUG
    int getch() { return fgetc(in); }
    int getch_unchecked() { return getch(); }
    int unget(int c = -1) { return ungetc(c, in); }
    int peek() { return unget(getch()); }
    void input(FILE* f) { in = f, set(); }
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        unget(ch);
    }
    void ireadstr(char* s, usize n) { fread(s, 1, n, in); }
#elif defined(USE_MMAP)
    void skipws() {
        while (blank(*ip) ECHK4)
            ip++;
        ECHK6
    }
    int unget(int = 0) = delete;
    int getch() { return ECHK0; }
    int getch_unchecked() { return *ip++; }
    int peek() { return *ip; }
    void input(FILE* f) {
        struct stat st;
        int fd;
        in = f;
        if (in)
            fd = fileno(in), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0), set();
    }
    void ireadstr(char* s, usize n) { memcpy(s, ip, n), ip += n; }
    static constexpr auto I = [] {
        std::array<u32, 0x10000> I{};
        fill(I, -1);
        _for (i, 10)
            _for (j, 10)
                I[i + j * 0x100 + 0x3030] = i * 10 + j;
        return I;
    }();
#else
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        ECHK5
        unget();
    }
    int unget(int = 0) { return *ip--; }
    int getch() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip; }
    void input(FILE* f) { in = f, ip = eip = ibuf, set(); }
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
#endif
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool s = true) { status = s; }
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
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
        while (!isdigit(*ip) ECHK4)
            sign = *ip++ == '-';
        ECHK2
        t = *ip++ ^ 48;
        while (~I[*reinterpret_cast<u16*&>(ip)])
            t = t * 100 + I[*reinterpret_cast<u16*&>(ip)++];
        if (isdigit(*ip))
            t = t * 10 + (*ip++ ^ 48);
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
        while (!isdigit(*ip) ECHK4)
            ip++;
        ECHK2
        x = *ip++ ^ 48;
        while (~I[*reinterpret_cast<u16*&>(ip)])
            x = x * 100 + I[*reinterpret_cast<u16*&>(ip)++];
        if (isdigit(*ip))
            x = x * 10 + (*ip++ ^ 48);
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
        char* ip = this->ip;
        while (!blank(*ip) ECHK4)
            ip++;
        return ip - this->ip;
    }
    IO& read(char* s) {
        skipws();
        auto n = next_size();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        auto n = next_size();
        s.assign(ip, n);
        ip += n;
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.assign(ip, n);
        ip += n;
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
        ireadstr(s.data(), n);
        return *this;
    }
#endif
    IO& readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
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
    template <std::forward_iterator I>
    IO& readArray(I f, I l) {
        while (f != l)
            read(*f++);
        return *this;
    }
    IO& readArray(forward_range auto&& r) { return readArray(all(r)); }
    IO& zipread(auto&&... a) {
        _for (i, (len(a), ...))
            read(a[i]...);
        return *this;
    }
    
#ifdef LX_DEBUG
    void flush() { fflush(out), set(); }
    void putch_unchecked(char c) { fputc(c, out); }
    void putch(char c) { putch_unchecked(c); }
    void writestr(const char* s, usize n) { fwrite(s, 1, n, out); }
#else
    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    void putch_unchecked(char c) { *op++ = c; }
    void putch(char c) { (op == end(obuf) ? flush() : void()), putch_unchecked(c); }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
    static constexpr auto O = [] {
        std::array<u32, 10000> O{};
        int x = 0;
        _for (i, 10)
            _for (j, 10)
                _for (k, 10)
                    _for (l, 10)
                        O[x++] = i + j * 0x100 + k * 0x10000 + l * 0x1000000 + 0x30303030;
        return O;
    }();
#endif
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { out = f; }
    void setprec(u32 n = 6) { prec = n; }
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
        if (end(obuf) - op < 64) [[unlikely]]
            flush();

        auto L = [&](int x) { return x == 1 ? 0 : ten(x - 1); };
        auto R = [&](int x) { return ten(x) - 1; };

#define de(t)                          \
    case L(t)... R(t):                 \
        *(u32*)op = O[x / ten((t)-4)]; \
        op += 4;                       \
        x %= ten((t)-4);               \
        [[fallthrough]]

        u64 y = x;
        switch (y) {
            de(18);
            de(14);
            de(10);
            de(6);
        case L(2)... R(2):
            *(u32*)op = O[x * 100];
            op += 2;
            break;

            de(17);
            de(13);
            de(9);
            de(5);
        case L(1)... R(1):
            *op++ = x ^ 48;
            break;

        default:
            *(u32*)op = O[x / ten(16)];
            op += 4;
            x %= ten(16);
            [[fallthrough]];
            de(16);
            de(12);
            de(8);
        case L(4)... R(4):
            *(u32*)op = O[x];
            op += 4;
            break;

            de(19);
            de(15);
            de(11);
            de(7);
        case L(3)... R(3):
            *(u32*)op = O[x * 10];
            op += 3;
            break;
        }
#undef de
#else
        write(u128(x));
#endif
    }
    void write(u128 x) {
#ifndef LX_DEBUG
        if (end(obuf) - op < 64) [[unlikely]]
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
        writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
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