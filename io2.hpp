#pragma once
#include "utility/itos_table.hpp"

#ifndef FASTIO
#define FASTIO 2

struct IO {
    static constexpr usize bufSize = 1 << 20;
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }
    static constexpr bool blank(int c) { return c <= ' '; }

    u32 prec = 12;
    FILE *in, *out;
    bool status;

    int getch() {
        char ch = fgetc(in);
        if (ch == EOF)
            return set(false), -1;
        return ch;
    }
    int getch_unchecked() { return getch(); }
    int unget(int c = -1) const { return ungetc(c, in); }
    int peek() { return unget(getch()); }
    void input(FILE* f) { in = f, set(); }
    void skipws() {
        int ch = getch();
        while (blank(ch) && *this)
            ch = getch();
        unget(ch);
    }
    void ireadstr(char* s, usize n) const { fread(s, 1, n, in); }
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool s = true) { status = s; }
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    IO& read(Args&... x) {
        (read(x) && ...);
        return *this;
    }
    template <Signed T>
    IO& read(T& x) {
        x = 0;
        make_unsigned_t<T> t;
        bool sign = false;
        int ch = getch();
        while (!isdigit(ch) && *this)
            sign = ch == '-', ch = getch();
        if (!*this)
            return *this;
        t = 0;
        while (isdigit(ch))
            t = t * 10 + (ch ^ 48), ch = getch();
        unget(ch);
        x = sign ? -t : t;
        return set(), *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
        int ch = getch();
        while (!isdigit(ch) && *this)
            ch = getch();
        if (!*this)
            return *this;
        while (isdigit(ch))
            x = x * 10 + (ch ^ 48), ch = getch();
        unget(ch);
        return set(), *this;
    }
    IO& read(std::floating_point auto& x) {
        static str s;
        if (read(s))
            std::from_chars(s.begin().base(), s.end().base(), x);
        return *this;
    }
    template <typename T = int>
    T read() {
        std::decay_t<T> x;
        return read(x), x;
    }
    IO& read(char& ch) {
        skipws();
        ch = getch();
        return *this;
    }
    IO& read(char* s) {
        skipws();
        int ch = peek();
        if (!*this)
            return *this;
        while (!blank(ch))
            *s++ = ch, getch_unchecked(), ch = peek();
        *s = 0;
        return set(), *this;
    }
    IO& read(str& s) {
        skipws();
        int ch = peek();
        if (!*this)
            return *this;
        s.clear();
        while (!blank(ch))
            s.push_back(ch), getch_unchecked(), ch = peek();
        return set(), *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        ireadstr(s.data(), n);
        return *this;
    }
    IO& readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& readline(char* s) {
        char* t = s;
        int ch = getch();
        while (ch != '\n' && *this)
            *s++ = ch, ch = getch();
        *s = 0;
        if (s != t)
            set();
        return *this;
    }
    IO& readline(str& s) {
        s.clear();
        int ch = getch();
        while (ch != '\n' && *this)
            s.push_back(ch), ch = getch();
        if (!s.empty())
            set();
        return *this;
    }
    IO& read(tupleLike auto& t) {
        return std::apply([&](auto&... t) { read(t...); }, t), *this;
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
    void flush() { fflush(out), set(); }
    void putch_unchecked(char c) const { fputc(c, out); }
    void putch(char c) const { putch_unchecked(c); }
    void writestr(const char* s, usize n) const { fwrite(s, 1, n, out); }
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { out = f; }
    void setprec(u32 n = 6) { prec = n; }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() const {}
    template <Signed T>
    void write(T x) const {
        make_unsigned_t<T> y = x;
        if (x < 0)
            write('-', y = -y);
        else
            write(y);
    }
    template <Unsigned T>
    void write(T x) const {
        static int s[40], t = 0;
        do
            s[t++] = x % 10, x /= 10;
        while (x);
        while (t)
            putch_unchecked(s[--t] ^ 48);
    }
    void write(char c) const { putch(c); }
    void write(std::floating_point auto x) const {
        static char buf[512];
        writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
    }
    void write(std::string_view s) const { writestr(s.data(), s.size()); }
    template <typename I, typename T = std::iter_value_t<I>>
    static constexpr char default_delim = tupleLike<T> || input_range<T> ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
        if (f != l)
            for (write(*f++); f != l; write(d, *f++)) {}
    }
    template <tupleLike T>
    void write(T&& t) {
        std::apply([&](auto&& x, auto&&... y) { write(FORWARD(x)), (write(' ', FORWARD(y)), ...); }, FORWARD(t));
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)...), print(); }
    void print() const { putch('\n'); }
    void print(auto&&... x) { write(std::forward_as_tuple(FORWARD(x)...), '\n'); }
    template <std::input_iterator I, std::sentinel_for<I> S>
    void displayArray(I f, S l, char d = default_delim<I>) { print_range(f, l, d), print(); }
    template <input_range R>
    void displayArray(R&& r, char d = default_delim<iterator_t<R>>) { displayArray(all(r), d); }
    operator bool() const { return status; }
} io;
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
inline void NO(bool v = true) { YES(!v); }
void Yes(bool v = true) { io.write(v ? "Yes\n" : "No\n"); }
inline void No(bool v = true) { Yes(!v); }
#endif