/**
 * @file    Complexity.h
 * @brief   Compile-time complexity annotation helpers and growth-class
 *          definitions used throughout the dsa-cpp curriculum.
 *
 * ┌─────────────────────────────────────────────────┐
 * │  This file has NO runtime cost.                 │
 * │  Everything here is constexpr / enum / macro.   │
 * └─────────────────────────────────────────────────┘
 *
 * Usage:
 *   // At the top of every .h file in this project:
 *   //   Time  – insert(), remove(), search() : O(?)
 *   //   Space – overall                      : O(?)
 */

#pragma once
#include <string_view>

namespace dsa {

// ── Growth classes ────────────────────────────────────────────────────────────
enum class Growth {
    Constant,       ///< O(1)
    Logarithmic,    ///< O(log n)
    Linear,         ///< O(n)
    Linearithmic,   ///< O(n log n)
    Quadratic,      ///< O(n²)
    Cubic,          ///< O(n³)
    Exponential,    ///< O(2ⁿ)
    Factorial       ///< O(n!)
};

/// Returns a human-readable label for a growth class.
constexpr std::string_view growth_label(Growth g) noexcept {
    switch (g) {
        case Growth::Constant:      return "O(1)";
        case Growth::Logarithmic:   return "O(log n)";
        case Growth::Linear:        return "O(n)";
        case Growth::Linearithmic:  return "O(n log n)";
        case Growth::Quadratic:     return "O(n²)";
        case Growth::Cubic:         return "O(n³)";
        case Growth::Exponential:   return "O(2ⁿ)";
        case Growth::Factorial:     return "O(n!)";
    }
    return "O(?)";
}

// ── Annotation macro (documents complexity in code, zero runtime cost) ────────
/**
 * @brief  Drop this comment block at the top of every data-structure header.
 *
 * Example:
 *   DSA_COMPLEXITY_TABLE(
 *       "push_back : O(1) amortised",
 *       "pop_back  : O(1)",
 *       "search    : O(n)",
 *       "Space     : O(n)"
 *   )
 *
 * It expands to nothing — it is purely documentary.
 */
#define DSA_COMPLEXITY_TABLE(...) /* complexity: __VA_ARGS__ */

} // namespace dsa