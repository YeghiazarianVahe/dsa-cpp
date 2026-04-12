/**
 * @file  tests/test_string.cpp
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       tests/test_string.cpp String.cpp -o test_string
 */

#include "../String.h"
#include <cassert>
#include <iostream>
#include <format>

static std::size_t tests_run    = 0;
static std::size_t tests_passed = 0;

#define CHECK(expr)                                                          \
    do {                                                                     \
        ++tests_run;                                                         \
        if (expr) { ++tests_passed; }                                        \
        else { std::cerr << "[FAIL] " << #expr                               \
                         << " (" << __FILE__ << ":" << __LINE__ << ")\n"; } \
    } while(false)

// ── Tests ─────────────────────────────────────────────────────────────────────

static void test_default_constructor() {
    dsa::String s;
    CHECK(s.size()     == 0);
    CHECK(s.empty()    == true);
    CHECK(s.c_str()[0] == '\0');
}

static void test_cstr_constructor() {
    dsa::String s{"hello"};
    CHECK(s.size()  == 5);
    CHECK(s.empty() == false);
    CHECK(s[0] == 'h');
    CHECK(s[4] == 'o');
}

static void test_nullptr_constructor() {
    dsa::String s{nullptr};
    CHECK(s.size()  == 0);
    CHECK(s.empty() == true);
}

static void test_copy_constructor() {
    dsa::String a{"world"};
    dsa::String b{a};
    CHECK(b.size() == 5);
    CHECK(b[0] == 'w');
    // must be independent
    b[0] = 'X';
    CHECK(a[0] == 'w');
}

static void test_move_constructor() {
    dsa::String a{"move"};
    dsa::String b{std::move(a)};
    CHECK(b.size()  == 4);
    CHECK(b[0]      == 'm');
    CHECK(a.size()  == 0);
    CHECK(a.empty() == true);
}

static void test_copy_assignment() {
    dsa::String a{"copy"};
    dsa::String b;
    b = a;
    CHECK(b.size() == 4);
    CHECK(b[0]     == 'c');
    b[0] = 'Z';
    CHECK(a[0]     == 'c');  // independent
}

static void test_move_assignment() {
    dsa::String a{"assign"};
    dsa::String b;
    b = std::move(a);
    CHECK(b.size() == 6);
    CHECK(a.size() == 0);
}

static void test_at_throws() {
    dsa::String s{"hi"};
    bool threw = false;
    try { s.at(10); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

static void test_reserve() {
    dsa::String s;
    s.reserve(64);
    CHECK(s.capacity() >= 64);
    CHECK(s.size()     == 0);
}

static void test_append_string() {
    dsa::String a{"hello"};
    dsa::String b{" world"};
    a.append(b);
    CHECK(a.size() == 11);
    CHECK(a == dsa::String{"hello world"});
}

static void test_append_cstr() {
    dsa::String a{"foo"};
    a.append("bar");
    CHECK(a.size() == 6);
    CHECK(a == dsa::String{"foobar"});
}

static void test_push_back() {
    dsa::String s;
    s.push_back('A');
    s.push_back('B');
    s.push_back('C');
    CHECK(s.size() == 3);
    CHECK(s[0] == 'A');
    CHECK(s[2] == 'C');
    CHECK(s.c_str()[3] == '\0');
}

static void test_clear() {
    dsa::String s{"nonempty"};
    s.clear();
    CHECK(s.size()     == 0);
    CHECK(s.empty()    == true);
    CHECK(s.c_str()[0] == '\0');
}

static void test_find_string() {
    dsa::String s{"hello world"};
    CHECK(s.find(dsa::String{"world"}) == 6);
    CHECK(s.find(dsa::String{"xyz"})   == dsa::String::npos);
    CHECK(s.find(dsa::String{"hello"}) == 0);
}

static void test_find_cstr() {
    dsa::String s{"abcabc"};
    CHECK(s.find("abc")    == 0);
    CHECK(s.find("abc", 1) == 3);
    CHECK(s.find("xyz")    == dsa::String::npos);
}

static void test_find_empty_pattern() {
    dsa::String s{"hello"};
    CHECK(s.find("") == 0);
}

static void test_substr() {
    dsa::String s{"hello world"};
    dsa::String sub = s.substr(6, 5);
    CHECK(sub.size() == 5);
    CHECK(sub == dsa::String{"world"});
}

static void test_substr_clamp() {
    dsa::String s{"hello"};
    dsa::String sub = s.substr(3, 100);  // only 2 chars available
    CHECK(sub.size() == 2);
    CHECK(sub == dsa::String{"lo"});
}

static void test_substr_throws() {
    dsa::String s{"hello"};
    bool threw = false;
    try { s.substr(10); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

static void test_operator_plus() {
    dsa::String a{"hello"};
    dsa::String b{" world"};
    dsa::String c = a + b;
    CHECK(c.size() == 11);
    CHECK(c == dsa::String{"hello world"});
    CHECK(a.size() == 5);  // a unchanged
}

static void test_operator_equals() {
    dsa::String a{"same"};
    dsa::String b{"same"};
    dsa::String c{"diff"};
    CHECK(a == b);
    CHECK(a != c);
}

static void test_operator_less() {
    dsa::String a{"apple"};
    dsa::String b{"banana"};
    CHECK(a < b);
    CHECK(!(b < a));
}

static void test_null_terminator_invariant() {
    dsa::String s;
    for (int i = 0; i < 50; ++i) s.push_back('x');
    CHECK(s.c_str()[50] == '\0');
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== String Tests ===\n\n";

    test_default_constructor();
    test_cstr_constructor();
    test_nullptr_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_at_throws();
    test_reserve();
    test_append_string();
    test_append_cstr();
    test_push_back();
    test_clear();
    test_find_string();
    test_find_cstr();
    test_find_empty_pattern();
    test_substr();
    test_substr_clamp();
    test_substr_throws();
    test_operator_plus();
    test_operator_equals();
    test_operator_less();
    test_null_terminator_invariant();

    std::cout << std::format("\nResult: {}/{} tests passed.\n",
                             tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}