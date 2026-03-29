/**
 * @file    Benchmark.h
 * @brief   Lightweight benchmarking engine for empirically measuring
 *          algorithm growth rates.
 *
 * Complexity of the benchmarker itself:
 *   measure()  – O(k·f(n))  where k = repetitions, f(n) = cost of the callable
 *   print_results() – O(S)  where S = number of sample sizes
 *   Space      – O(S)       stores one Result per sample size
 *
 * Design notes:
 *   - Uses std::chrono::high_resolution_clock for timing.
 *   - Accepts any callable via a template parameter (lambda, functor, fn ptr).
 *   - The callable receives the current sample size (std::size_t n) as its
 *     only argument and must set up + tear down its own data.
 *   - "Warm-up" runs are performed before measurement to reduce cache effects.
 */

#pragma once

#include "Complexity.h"

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <cmath>
#include <algorithm>

namespace dsa {

// ── Result record ─────────────────────────────────────────────────────────────
struct BenchResult {
    std::size_t n;          ///< Sample size
    double      ns_per_op;  ///< Nanoseconds per operation (averaged)
};

// ── Benchmark engine ──────────────────────────────────────────────────────────
class Benchmark {
public:
    /**
     * @brief  Construct a benchmark with a display name and expected growth.
     * @param  name          Human-readable label (e.g., "linear_scan").
     * @param  expected      The theoretical growth class (for display).
     * @param  repetitions   How many times to repeat each measurement.
     * @param  warmup_runs   Runs discarded before timing starts.
     */
    explicit Benchmark(std::string        name,
                       Growth             expected    = Growth::Linear,
                       std::size_t        repetitions = 5,
                       std::size_t        warmup_runs = 2)
        : name_{std::move(name)}
        , expected_{expected}
        , reps_{repetitions}
        , warmup_{warmup_runs}
    {}

    /**
     * @brief  Run the callable for each size in `sizes` and record timing.
     * @tparam Fn   Callable with signature void(std::size_t n).
     * @param  fn   The function under test.
     * @param  sizes  Vector of input sizes to benchmark.
     */
    template<typename Fn>
    void measure(Fn&& fn, const std::vector<std::size_t>& sizes) {
        results_.clear();
        results_.reserve(sizes.size());

        for (std::size_t n : sizes) {
            // Warm-up: run without timing to populate caches
            for (std::size_t w = 0; w < warmup_; ++w) {
                fn(n);
            }

            // Timed runs
            auto total_ns = 0.0;
            for (std::size_t r = 0; r < reps_; ++r) {
                auto t0 = std::chrono::high_resolution_clock::now();
                fn(n);
                auto t1 = std::chrono::high_resolution_clock::now();

                total_ns += static_cast<double>(
                    std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()
                );
            }

            results_.push_back({n, total_ns / static_cast<double>(reps_)});
        }
    }

    /**
     * @brief  Print a formatted results table to stdout.
     *         Also prints the empirical ratio between consecutive rows so you
     *         can visually identify the growth class:
     *           ratio ≈ 1        → O(1)
     *           ratio ≈ log₂(2)  → O(log n)  [when sizes double]
     *           ratio ≈ 2        → O(n)
     *           ratio ≈ 2 log 2  → O(n log n)
     *           ratio ≈ 4        → O(n²)
     */
    void print_results() const {
        if (results_.empty()) {
            std::cout << "[Benchmark] No results to display.\n";
            return;
        }

        // Header
        std::cout << std::format("\n+-- Benchmark: {:s}  Expected: {:s} --+\n",
                                 name_, growth_label(expected_));
        std::cout << std::format("| {:>12s}  {:>16s}  {:>12s}\n",
                                 "n", "ns / op", "ratio");
        std::cout << "+" << std::string(46, '-') << "+\n";

        for (std::size_t i = 0; i < results_.size(); ++i) {
            const auto& r = results_[i];
            std::string ratio_str = "    -";

            if (i > 0 && results_[i - 1].ns_per_op > 0.0) {
                double ratio = r.ns_per_op / results_[i - 1].ns_per_op;
                ratio_str = std::format("{:>8.2f}x", ratio);
            }

            std::cout << std::format("| {:>12d}  {:>14.1f} ns  {:>12s}\n",
                                     r.n, r.ns_per_op, ratio_str);
        }

        std::cout << "+" << std::string(46, '-') << "+\n";
    }
    /// Returns the raw results for programmatic inspection (e.g., in tests).
    [[nodiscard]] const std::vector<BenchResult>& results() const noexcept {
        return results_;
    }

    /// Returns the name of this benchmark.
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

private:
    std::string              name_;
    Growth                   expected_;
    std::size_t              reps_;
    std::size_t              warmup_;
    std::vector<BenchResult> results_;
};

} // namespace dsa