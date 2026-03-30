# Phase 1 / Topic 1 — Complexity Analysis

## What this module does
A lightweight **benchmarking harness** that empirically measures algorithm
growth rates and compares them to their theoretical Big-O bounds.
No external dependencies — pure C++20.

---

## Concepts covered

| Notation | Meaning | Typical example |
|----------|---------|-----------------|
| O(f(n)) | Upper bound — worst case | Always document this |
| Omega(f(n)) | Lower bound — best case | Proves minimum cost |
| Theta(f(n)) | Tight bound — both directions | When best == worst |

### Growth class cheat-sheet

| Class | Name | Ratio when n doubles | Example |
|-------|------|----------------------|---------|
| O(1) | Constant | ~1x | Array index lookup |
| O(log n) | Logarithmic | ~1.x | Binary search |
| O(n) | Linear | ~2x | Linear scan |
| O(n log n) | Linearithmic | ~2.x | Merge sort |
| O(n^2) | Quadratic | ~4x | Bubble sort |
| O(n^3) | Cubic | ~8x | Naive matrix multiply |
| O(2^n) | Exponential | explodes | Naive Fibonacci |
| O(n!) | Factorial | explodes | Brute-force TSP |

> **Key insight:** the "ratio" column is exactly what `Benchmark::print_results()`
> shows you. If you double n and the time ratio is ~4x, you have O(n^2).

---

## File layout

```
phase1/complexity_analysis/
├── CMakeLists.txt
├── Complexity.h                  <- growth enum + DSA_COMPLEXITY_TABLE macro
├── Benchmark.h                   <- template measurement engine
├── Benchmark.cpp                 <- translation unit (non-template helpers)
├── tests/
│   └── test_complexity.cpp       <- 20 assertions, custom test runner
└── examples/
    └── complexity_demo.cpp       <- live demo of O(1) through O(n^2)
```

---

## Complexity of the harness itself

| Operation | Time | Space |
|-----------|------|-------|
| `measure(fn, sizes)` | O(k * f(n)) — k = repetitions | O(S) — S = number of sizes |
| `print_results()` | O(S) | O(1) |

---

## Key classes & APIs

### `dsa::Growth` (Complexity.h)
```cpp
enum class Growth {
    Constant, Logarithmic, Linear, Linearithmic,
    Quadratic, Cubic, Exponential, Factorial
};

constexpr std::string_view growth_label(Growth g) noexcept;
```

### `DSA_COMPLEXITY_TABLE` macro (Complexity.h)
```cpp
// Drop this at the top of every data-structure header:
DSA_COMPLEXITY_TABLE(
    "push_back : O(1) amortised",
    "search    : O(n)",
    "Space     : O(n)"
)
// Expands to nothing — purely documentary.
```

### `dsa::Benchmark` (Benchmark.h)
```cpp
Benchmark b{"my_algorithm", dsa::Growth::Linear, /*reps=*/5, /*warmup=*/2};

b.measure([](std::size_t n) {
    // set up + run your algorithm for input size n
}, {1000, 2000, 4000, 8000});

b.print_results();   // prints table with ratio column
```

---

## Compile & run

### g++ (from phase1/complexity_analysis/)
```bash
# Tests
g++ -std=c++20 -Wall -Wextra -O2 \
    tests/test_complexity.cpp Benchmark.cpp -o test_complexity
./test_complexity

# Demo
g++ -std=c++20 -Wall -Wextra -O2 \
    examples/complexity_demo.cpp Benchmark.cpp -o complexity_demo
./complexity_demo
```

### CMake (from repo root)
```bash
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
cmake --build build
./build/phase1/complexity_analysis/test_complexity.exe
./build/phase1/complexity_analysis/complexity_demo.exe
```

---

## Expected test output
```
=== Complexity Analysis Tests ===

Result: 20/20 tests passed.
```

## Expected demo output (ratios vary by machine)
```
+-- Benchmark: constant  fn_constant()  Expected: O(1) --+
|            n          ns / op         ratio
+----------------------------------------------+
|       100000          120.3 ns            -
|       200000          118.7 ns        0.99x
|       400000          121.0 ns        1.02x
|       800000          119.4 ns        0.99x
+----------------------------------------------+

+-- Benchmark: quadratic  bubble_sort()  Expected: O(n^2) --+
|            n          ns / op         ratio
+----------------------------------------------+
|          500       145200.0 ns            -
|         1000       582100.0 ns        4.01x
|         2000      2330000.0 ns        4.00x
|         4000      9318000.0 ns        3.99x
+----------------------------------------------+
```

---

## Pitfalls

1. **Optimizer removes your benchmark loop.** If the result is unused the
   compiler deletes the loop entirely. Fix: store output in a `volatile`
   variable so the compiler must keep the computation.

2. **Confusing average vs worst case.** Quicksort is O(n log n) average
   but O(n^2) worst case. Always state which scenario you mean.

3. **Adding complexities wrong.** O(n) + O(n^2) = O(n^2), not O(n^3).
   Keep the dominant term. Nesting loops multiplies: O(n) * O(n) = O(n^2).

---

## STL equivalent

| Our tool | STL / library equivalent |
|----------|--------------------------|
| `Benchmark` timer | `std::chrono::high_resolution_clock` directly |
| Growth labels | None built-in — always document manually |
| Full harness | Google Benchmark (for production perf work) |

---

## Status
✅ complete — committed in `feat(phase1/complexity_analysis): implement Benchmark harness with tests and demo`