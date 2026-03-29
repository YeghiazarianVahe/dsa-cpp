/**
 * @file  examples/complexity_demo.cpp
 * @brief Interactive demo: empirically observe O(1), O(log n), O(n),
 *        O(n log n), and O(n²) side-by-side.
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       examples/complexity_demo.cpp ../Benchmark.cpp -o complexity_demo
 *   cl /std:c++20 /W4 /O2 examples\complexity_demo.cpp ..\Benchmark.cpp
 */

#include "../Benchmark.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

// ── Sample functions under test ───────────────────────────────────────────────

/// O(1) — read the first element
static void fn_constant(std::size_t n) {
    std::vector<int> v(n, 7);
    volatile int x = v[0];
    (void)x;
}

/// O(log n) — binary search in a sorted array
static void fn_logarithmic(std::size_t n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);          // fill 0..n-1
    volatile bool found = std::binary_search(v.begin(), v.end(),
                                             static_cast<int>(n / 2));
    (void)found;
}

/// O(n) — linear sum
static void fn_linear(std::size_t n) {
    std::vector<long long> v(n, 1LL);
    volatile long long s = std::accumulate(v.begin(), v.end(), 0LL);
    (void)s;
}

/// O(n log n) — std::sort on a random-ish array
static void fn_linearithmic(std::size_t n) {
    std::vector<int> v(n);
    // Simple deterministic "random" fill to avoid needing <random>
    for (std::size_t i = 0; i < n; ++i)
        v[i] = static_cast<int>((i * 2654435761ULL) >> 16);
    std::sort(v.begin(), v.end());
}

/// O(n²) — bubble sort (the real one, not std::sort)
static void fn_quadratic(std::size_t n) {
    std::vector<int> v(n);
    for (std::size_t i = 0; i < n; ++i)
        v[i] = static_cast<int>(n - i);   // worst case: reverse sorted

    // Classic bubble sort — guaranteed O(n²)
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j + 1 < n - i; ++j)
            if (v[j] > v[j + 1])
                std::swap(v[j], v[j + 1]);
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "+==================================================+\n";
    std::cout << "|     DSA-CPP  .  Complexity Analysis Demo         |\n";
    std::cout << "|  Watch the 'ratio' column to identify growth     |\n";
    std::cout << "|  O(1)->1x  O(n)->2x  O(n^2)->4x  (sizes double) |\n";
    std::cout << "+==================================================+\n";
    
    const std::vector<std::size_t> small_sizes  = {500,  1000, 2000, 4000};
    const std::vector<std::size_t> medium_sizes = {5000, 10000, 20000, 40000};
    const std::vector<std::size_t> large_sizes  = {100000, 200000, 400000, 800000};

    // O(1)
    {
        dsa::Benchmark b{"constant  fn_constant()", dsa::Growth::Constant, 8, 3};
        b.measure(fn_constant, large_sizes);
        b.print_results();
    }

    // O(log n)
    {
        dsa::Benchmark b{"logarithmic  binary_search()", dsa::Growth::Logarithmic, 8, 3};
        b.measure(fn_logarithmic, large_sizes);
        b.print_results();
    }

    // O(n)
    {
        dsa::Benchmark b{"linear  accumulate()", dsa::Growth::Linear, 8, 3};
        b.measure(fn_linear, large_sizes);
        b.print_results();
    }

    // O(n log n)
    {
        dsa::Benchmark b{"linearithmic  std::sort()", dsa::Growth::Linearithmic, 6, 2};
        b.measure(fn_linearithmic, medium_sizes);
        b.print_results();
    }

    // O(n²)
    {
        dsa::Benchmark b{"quadratic  bubble_sort()", dsa::Growth::Quadratic, 5, 2};
        b.measure(fn_quadratic, small_sizes);
        b.print_results();
    }

    std::cout << "\n[DSA-CPP] Demo complete.\n";
    return 0;
}