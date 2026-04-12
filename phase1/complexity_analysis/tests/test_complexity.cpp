/**
 * @file  tests/test_complexity.cpp
 * @brief Unit tests for Complexity.h and Benchmark.h
 *
 * Tests:
 *   1. growth_label() returns correct strings for every enum value.
 *   2. Benchmark measures an O(1) function and confirms ratio ≈ 1.
 *   3. Benchmark measures an O(n) function and confirms ratio ≈ 2
 *      when input doubles.
 *   4. Benchmark measures an O(n²) function and confirms ratio ≈ 4
 *      when input doubles.
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       tests/test_complexity.cpp Benchmark.cpp -o test_complexity
 *   cl /std:c++20 /W4 /O2 tests\test_complexity.cpp Benchmark.cpp
 */

#include "../Complexity.h"
#include "../Benchmark.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// ── Tiny test runner ──────────────────────────────────────────────────────────
static std::size_t tests_run    = 0;
static std::size_t tests_passed = 0;

#define CHECK(expr)                                                          \
    do {                                                                     \
        ++tests_run;                                                         \
        if (expr) {                                                          \
            ++tests_passed;                                                  \
        } else {                                                             \
            std::cerr << "[FAIL] " << #expr                                  \
                      << "  (" << __FILE__ << ":" << __LINE__ << ")\n";     \
        }                                                                    \
    } while(false)

// ── Helpers ───────────────────────────────────────────────────────────────────
/// Returns true if `actual` is within `pct`% of `expected`.
static bool near(double actual, double expected, double pct = 30.0) {
    return std::fabs(actual - expected) <= (pct / 100.0) * expected;
}

// ── Test 1: growth_label ──────────────────────────────────────────────────────
static void test_growth_labels() {
    using dsa::Growth;
    using dsa::growth_label;

    CHECK(growth_label(Growth::Constant)     == "O(1)");
    CHECK(growth_label(Growth::Logarithmic)  == "O(log n)");
    CHECK(growth_label(Growth::Linear)       == "O(n)");
    CHECK(growth_label(Growth::Linearithmic) == "O(n log n)");
    CHECK(growth_label(Growth::Quadratic)    == "O(n²)");
    CHECK(growth_label(Growth::Cubic)        == "O(n³)");
    CHECK(growth_label(Growth::Exponential)  == "O(2ⁿ)");
    CHECK(growth_label(Growth::Factorial)    == "O(n!)");
}

// ── Test 2: O(1) function — ratio must stay near 1.0 ─────────────────────────
static void test_constant_growth() {
    dsa::Benchmark b{"O(1) array access", dsa::Growth::Constant, 10, 3};

    // Pre-built array; lambda just reads element [0] — pure O(1)
    std::vector<int> data(1'000'000, 42);

    b.measure([&](std::size_t /*n*/) {
        volatile int x = data[0];   // volatile prevents the optimizer
        (void)x;                    // from removing the "dead" read
    }, {1000, 2000, 4000, 8000, 16000});

    // Every consecutive ratio should be near 1.0 (within 60% — timing noise)
    const auto& res = b.results();
    for (std::size_t i = 1; i < res.size(); ++i) {
        if (res[i - 1].ns_per_op > 0.0) {
            double ratio = res[i].ns_per_op / res[i - 1].ns_per_op;
            CHECK(near(ratio, 1.0, 80.0));
        }
    }
}

// ── Test 3: O(n) function — ratio must be near 2.0 when n doubles ────────────
static void test_linear_growth() {
    dsa::Benchmark b{"O(n) linear sum", dsa::Growth::Linear, 5, 2};

    b.measure([](std::size_t n) {
        // Sum a freshly built vector of size n — touches every element once
        std::vector<long long> v(n, 1LL);
        volatile long long s = std::accumulate(v.begin(), v.end(), 0LL);
        (void)s;
    }, {10'000, 20'000, 40'000, 80'000});

    const auto& res = b.results();
    for (std::size_t i = 1; i < res.size(); ++i) {
        if (res[i - 1].ns_per_op > 0.0) {
            double ratio = res[i].ns_per_op / res[i - 1].ns_per_op;
            // Should be near 2.0; allow 50% tolerance for OS scheduling noise
            CHECK(near(ratio, 2.0, 50.0));
        }
    }
}

// ── Test 4: O(n²) function — ratio must be near 4.0 when n doubles ───────────
static void test_quadratic_growth() {
    dsa::Benchmark b{"O(n²) nested loop", dsa::Growth::Quadratic, 5, 2};

    b.measure([](std::size_t n) {
        volatile long long s = 0;
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                s += static_cast<long long>(i * j);
        (void)s;
    }, {200, 400, 800, 1600});

    const auto& res = b.results();
    for (std::size_t i = 1; i < res.size(); ++i) {
        if (res[i - 1].ns_per_op > 0.0) {
            double ratio = res[i].ns_per_op / res[i - 1].ns_per_op;
            CHECK(near(ratio, 4.0, 40.0));
        }
    }
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== Complexity Analysis Tests ===\n\n";

    test_growth_labels();
    test_constant_growth();
    test_linear_growth();
    test_quadratic_growth();

    std::cout << std::format("\nResult: {}/{} tests passed.\n",
                             tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}