/**
 * @file  tests/test_dynamic_array.cpp
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       tests/test_dynamic_array.cpp DynamicArray.cpp -o test_dynamic_array
 */

#include "../DynamicArray.h"
#include <cassert>
#include <iostream>
#include <format>

static std::size_t tests_run    = 0;
static std::size_t tests_passed = 0;

#define CHECK(expr)                                                      \
    do {                                                                 \
        ++tests_run;                                                     \
        if (expr) { ++tests_passed; }                                   \
        else { std::cerr << "[FAIL] " << #expr                          \
                         << " (" << __FILE__ << ":" << __LINE__ << ")\n"; } \
    } while(false)

// ── Tests ─────────────────────────────────────────────────────────────────────

static void test_default_constructor() {
    dsa::DynamicArray<int> a;
    CHECK(a.size()     == 0);
    CHECK(a.capacity() == 0);
    CHECK(a.empty()    == true);
}

static void test_explicit_capacity_constructor() {
    dsa::DynamicArray<int> a(8);
    CHECK(a.size()     == 0);
    CHECK(a.capacity() == 8);
    CHECK(a.empty()    == true);
}

static void test_push_back_and_access() {
    dsa::DynamicArray<int> a;
    a.push_back(10);
    a.push_back(20);
    a.push_back(30);
    CHECK(a.size()  == 3);
    CHECK(a[0]      == 10);
    CHECK(a[1]      == 20);
    CHECK(a[2]      == 30);
    CHECK(a.front() == 10);
    CHECK(a.back()  == 30);
}

static void test_grow_on_push_back() {
    dsa::DynamicArray<int> a;
    for (int i = 0; i < 100; ++i)
        a.push_back(i);
    CHECK(a.size() == 100);
    CHECK(a[0]  == 0);
    CHECK(a[99] == 99);
    CHECK(a.capacity() >= 100);
}

static void test_at_throws() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    bool threw = false;
    try { a.at(5); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

static void test_pop_back() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    a.pop_back();
    CHECK(a.size() == 1);
    CHECK(a[0]     == 1);
}

static void test_insert() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    a.push_back(3);
    a.insert(1, 2);          // [1, 2, 3]
    CHECK(a.size() == 3);
    CHECK(a[0] == 1);
    CHECK(a[1] == 2);
    CHECK(a[2] == 3);
}

static void test_erase() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.erase(1);              // [1, 3]
    CHECK(a.size() == 2);
    CHECK(a[0] == 1);
    CHECK(a[1] == 3);
}

static void test_clear() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    a.clear();
    CHECK(a.size()  == 0);
    CHECK(a.empty() == true);
}

static void test_reserve() {
    dsa::DynamicArray<int> a;
    a.reserve(64);
    CHECK(a.capacity() >= 64);
    CHECK(a.size()     == 0);
}

static void test_resize_grow() {
    dsa::DynamicArray<int> a;
    a.push_back(1);
    a.resize(4, 99);
    CHECK(a.size() == 4);
    CHECK(a[0] == 1);
    CHECK(a[1] == 99);
    CHECK(a[3] == 99);
}

static void test_resize_shrink() {
    dsa::DynamicArray<int> a;
    for (int i = 0; i < 5; ++i) a.push_back(i);
    a.resize(2);
    CHECK(a.size() == 2);
    CHECK(a[0] == 0);
    CHECK(a[1] == 1);
}

static void test_copy_constructor() {
    dsa::DynamicArray<int> a;
    a.push_back(1); a.push_back(2); a.push_back(3);
    dsa::DynamicArray<int> b{a};
    CHECK(b.size() == 3);
    CHECK(b[0] == 1 && b[1] == 2 && b[2] == 3);
    // must be independent
    b[0] = 99;
    CHECK(a[0] == 1);   // original unchanged
}

static void test_move_constructor() {
    dsa::DynamicArray<int> a;
    a.push_back(7); a.push_back(8);
    dsa::DynamicArray<int> b{std::move(a)};
    CHECK(b.size() == 2);
    CHECK(b[0] == 7 && b[1] == 8);
    CHECK(a.size() == 0);       // moved-from is empty
}

static void test_copy_assignment() {
    dsa::DynamicArray<int> a;
    a.push_back(10); a.push_back(20);
    dsa::DynamicArray<int> b;
    b = a;
    CHECK(b.size() == 2);
    CHECK(b[0] == 10);
    b[0] = 99;
    CHECK(a[0] == 10);  // independent
}

static void test_move_assignment() {
    dsa::DynamicArray<int> a;
    a.push_back(5); a.push_back(6);
    dsa::DynamicArray<int> b;
    b = std::move(a);
    CHECK(b.size() == 2);
    CHECK(b[0] == 5);
    CHECK(a.size() == 0);
}

static void test_iterator() {
    dsa::DynamicArray<int> a;
    for (int i = 0; i < 5; ++i) a.push_back(i * 10);
    int expected = 0;
    for (int val : a) {
        CHECK(val == expected);
        expected += 10;
    }
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== DynamicArray Tests ===\n\n";

    test_default_constructor();
    test_explicit_capacity_constructor();
    test_push_back_and_access();
    test_grow_on_push_back();
    test_at_throws();
    test_pop_back();
    test_insert();
    test_erase();
    test_clear();
    test_reserve();
    test_resize_grow();
    test_resize_shrink();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_iterator();

    std::cout << std::format("\nResult: {}/{} tests passed.\n",
                             tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}