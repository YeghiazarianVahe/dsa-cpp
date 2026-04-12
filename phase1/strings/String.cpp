#include "String.h"
#include <stdexcept>
#include <algorithm>

namespace dsa {

// Add these private helpers at the top of the dsa namespace, before any constructor:

static std::size_t dsa_strlen(const char* s) noexcept {
    std::size_t n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

static void dsa_memcpy(char* dst, const char* src, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) dst[i] = src[i];
}

static int dsa_memcmp(const char* a, const char* b, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

static int dsa_strcmp(const char* a, const char* b) noexcept {
    while (*a && *a == *b) { ++a; ++b; }
    return (unsigned char)*a - (unsigned char)*b;
}


// ── Constructors & Destructor ─────────────────────────

String::String()
    : data_{new char[1]{'\0'}}, size_{0}, capacity_{0} {}

String::String(const char* cstr) {
    if (cstr == nullptr) {
        size_ = capacity_ = 0;
        data_ = new char[1]{'\0'};
        return;
    }
    size_ = capacity_ = dsa_strlen(cstr);
    data_ = new char[capacity_ + 1];
    dsa_memcpy(data_, cstr, size_);
    data_[size_] = '\0';
}

String::String(const String& other)
    : size_{other.size_}, capacity_{other.capacity_} {
    data_ = new char[capacity_ + 1];
    dsa_memcpy(data_, other.data_, size_);
    data_[size_] = '\0';
}

String::String(String&& other) noexcept
    : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} {
    // Leave the moved-from object in a valid, empty state
    other.data_ = new char[1]{'\0'};
    other.size_ = 0;
    other.capacity_ = 0;
}

String::~String() {
    delete[] data_;
}

// ── Assignments ───────────────────────────────────────

String& String::operator=(const String& other) {
    if (this != &other) {
        char* new_data = new char[other.capacity_ + 1];
        dsa_memcpy(new_data, other.data_, other.size_ + 1); // +1 copies the '\0'
        delete[] data_;
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;
    }
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        
        other.data_ = new char[1]{'\0'};
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

// ── Element Access ────────────────────────────────────

char& String::operator[](std::size_t pos) { return data_[pos]; }
const char& String::operator[](std::size_t pos) const { return data_[pos]; }

char& String::at(std::size_t pos) {
    if (pos >= size_) throw std::out_of_range("String::at index out of bounds");
    return data_[pos];
}

const char& String::at(std::size_t pos) const {
    if (pos >= size_) throw std::out_of_range("String::at index out of bounds");
    return data_[pos];
}

const char* String::c_str() const noexcept { return data_; }

// ── Capacity ──────────────────────────────────────────

std::size_t String::size() const noexcept { return size_; }
std::size_t String::capacity() const noexcept { return capacity_; }
bool String::empty() const noexcept { return size_ == 0; }

void String::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) return;
    char* new_data = new char[new_capacity + 1];
    dsa_memcpy(new_data, data_, size_ + 1);
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

// ── Modifiers ─────────────────────────────────────────

void String::grow(std::size_t min_capacity) {
    std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    while (new_capacity < min_capacity) {
        new_capacity *= 2;
    }
    reserve(new_capacity);
}

void String::push_back(char c) {
    if (size_ + 1 > capacity_) {
        grow(size_ + 1);
    }
    data_[size_] = c;
    ++size_;
    data_[size_] = '\0';
}

void String::clear() {
    size_ = 0;
    data_[0] = '\0';
}


void String::append(const String& other) {
    append(other.c_str(), other.size());
}

void String::append(const char* cstr) {
    if (!cstr) return;
    append(cstr, dsa_strlen(cstr));
}

void String::append(const char* str, std::size_t len) {
    if (size_ + len > capacity_) {
        grow(size_ + len);
    }
    dsa_memcpy(data_ + size_, str, len);
    size_ += len;
    data_[size_] = '\0';
}

// ── Search & Substring ────────────────────────────────

std::size_t String::find(const String& str, std::size_t start) const noexcept{
    return find(str.c_str(), start);
}

std::size_t String::find(const char* str, std::size_t start) const noexcept{
    if (!str || start > size_) return static_cast<std::size_t>(-1); // typically std::string::npos
    std::size_t len = dsa_strlen(str);
    if (len == 0) return start;
    if (len > size_ - start) return static_cast<std::size_t>(-1);

    // Naive O(N*M) string matching
    for (std::size_t i = start; i <= size_ - len; ++i) {
        if (dsa_memcmp(data_ + i, str, len) == 0) {
            return i;
        }
    }
    return static_cast<std::size_t>(-1);
}

String String::substr(std::size_t pos, std::size_t len) const {
    if (pos > size_) throw std::out_of_range("String::substr pos out of bounds");
    std::size_t actual_len = std::min(len, size_ - pos);
    
    String result;
    result.reserve(actual_len);
    dsa_memcpy(result.data_, data_ + pos, actual_len);
    result.size_ = actual_len;
    result.data_[actual_len] = '\0';
    return result;
}

// ── Operators ─────────────────────────────────────────

String operator+(const String& lhs, const String& rhs) {
    String result = lhs;
    result.append(rhs);
    return result;
}

bool operator==(const String& lhs, const String& rhs) noexcept{
    if (lhs.size() != rhs.size()) return false;
    return dsa_memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
}

bool operator!=(const String& lhs, const String& rhs) noexcept{
    return !(lhs == rhs);
}

bool operator<(const String& lhs, const String& rhs) noexcept{
    int cmp = dsa_strcmp(lhs.c_str(), rhs.c_str());
    return cmp < 0;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.data_;
    return os;
}

} // namespace dsa
