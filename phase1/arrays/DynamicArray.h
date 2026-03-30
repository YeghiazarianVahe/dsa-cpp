#pragma once
#include <cstddef>
#include <stdexcept>

// DSA_COMPLEXITY_TABLE(...)  <- fill this in

namespace dsa {

template<typename T>
class DynamicArray {
public:
    // ── Constructors / Rule of Five ───────────────────
    DynamicArray();                                // default
    explicit DynamicArray(std::size_t capacity);   // reserve upfront
    DynamicArray(const DynamicArray& other);       // copy ctor
    DynamicArray(DynamicArray&& other) noexcept;   // move ctor
    ~DynamicArray();                               // destructor

    DynamicArray& operator=(const DynamicArray& other);       // copy assign
    DynamicArray& operator=(DynamicArray&& other) noexcept;   // move assign

    // ── Element access ────────────────────────────────
    T&       operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
    T&       at(std::size_t index);          // throws std::out_of_range
    const T& at(std::size_t index) const;
    T&       front();
    T&       back();

    // ── Modifiers ─────────────────────────────────────
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void insert(std::size_t index, const T& value);
    void erase(std::size_t index);
    void clear();

    // ── Capacity ──────────────────────────────────────
    void        reserve(std::size_t new_capacity);
    void        resize(std::size_t new_size, const T& fill = T{});
    std::size_t size()     const noexcept;
    std::size_t capacity() const noexcept;
    bool        empty()    const noexcept;

    // ── Iterator (raw pointer style) ──────────────────
    T*       begin() noexcept;
    T*       end()   noexcept;
    const T* begin() const noexcept;
    const T* end()   const noexcept;

private:
    T*          data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow();   // double capacity, reallocate, copy, free old
};

} // namespace dsa