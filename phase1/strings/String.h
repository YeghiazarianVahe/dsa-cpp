#pragma once
#include <cstddef>
#include <stdexcept>
#include <ostream>

// DSA_COMPLEXITY_TABLE(...)  <- fill this in

namespace dsa {

class String {
public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    // ── Constructors / Rule of Five ───────────────────
    String();                            // empty string
    String(const char* cstr);            // from C-string literal
    String(const String& other);         // copy ctor
    String(String&& other) noexcept;     // move ctor
    ~String();                           // destructor

    String& operator=(const String& other);       // copy assign
    String& operator=(String&& other) noexcept;   // move assign

    // ── Element access ────────────────────────────────
    char&       operator[](std::size_t index);
    const char& operator[](std::size_t index) const;
    char&       at(std::size_t index);
    const char& at(std::size_t index) const;
    const char* c_str() const noexcept;   // null-terminated buffer

    // ── Capacity ──────────────────────────────────────
    std::size_t size()     const noexcept;
    std::size_t capacity() const noexcept;
    bool        empty()    const noexcept;
    void        reserve(std::size_t new_capacity);

    // ── Modifiers ─────────────────────────────────────
    void   append(const String& other);
    void   append(const char* cstr);
    void   push_back(char c);
    void   clear();

    // ── Search ────────────────────────────────────────
    std::size_t find(const String& pattern, std::size_t start = 0) const noexcept;
    std::size_t find(const char* pattern,   std::size_t start = 0) const noexcept;

    // ── Substrings ────────────────────────────────────
    String substr(std::size_t pos, std::size_t len = npos) const;

    // ── Operators ─────────────────────────────────────
    friend String operator+(const String& lhs, const String& rhs);

    friend bool   operator==(const String& lhs, const String& rhs) noexcept;
    friend bool   operator!=(const String& lhs, const String& rhs) noexcept;
    friend bool   operator<(const String& lhs,  const String& rhs) noexcept;
    // ── Stream output ─────────────────────────────────
    friend std::ostream& operator<<(std::ostream& os, const String& s);

private:
    char*       data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow(std::size_t min_capacity);  
    void append(const char* str, std::size_t len);  // add this line
};

} // namespace dsa