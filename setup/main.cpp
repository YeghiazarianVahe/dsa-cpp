/**
 * @file    setup/main.cpp
 * @brief   Toolchain verification — confirms C++20 is active.
 *
 * Compile (g++):  g++ -std=c++20 -Wall -Wextra -O2 main.cpp -o hello_dsa
 * Compile (MSVC): cl /std:c++20 /W4 /O2 main.cpp
 * Run:            ./hello_dsa   (or hello_dsa.exe on Windows)
 */

#include <iostream>
#include <string_view>   // C++17 — warms up modern headers
#include <format>        // C++20 — the real test

// ── Compile-time C++20 guard ─────────────────────────────────────────────────
static_assert(__cplusplus >= 202002L,
    "This project requires C++20 or later. "
    "Pass -std=c++20 (g++) or /std:c++20 (MSVC).");

// ── Detect which compiler we are running under ───────────────────────────────
constexpr std::string_view detect_compiler() noexcept {
#if defined(_MSC_VER)
    return "MSVC";
#elif defined(__GNUC__)
    return "GCC/g++";
#elif defined(__clang__)
    return "Clang";
#else
    return "Unknown";
#endif
}

int main() {
    // std::format is a C++20 feature — if this compiles, C++20 is confirmed.
    const std::string msg = std::format(
        "[DSA-CPP] Environment OK. Compiler: {}  C++20 ready.",
        detect_compiler()
    );

    std::cout << msg << '\n';
    return 0;
}
