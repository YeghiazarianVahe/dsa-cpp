/**
 * @file  examples/string_demo.cpp
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       examples/string_demo.cpp ../String.cpp -o string_demo
 */

#include "../String.h"
#include <iostream>

int main() {
    std::cout << "=== String Demo ===\n\n";

    // ── Construction ──────────────────────────────────
    std::cout << "-- Construction --\n";
    dsa::String a{"hello"};
    dsa::String b{" world"};
    std::cout << "a = " << a << "  size=" << a.size() << "\n";
    std::cout << "b = " << b << "  size=" << b.size() << "\n";

    // ── Append & operator+ ────────────────────────────
    std::cout << "\n-- Append & operator+ --\n";
    dsa::String c = a + b;
    std::cout << "a + b = " << c << "\n";
    c.append("!");
    std::cout << "after append(\"!\") = " << c << "\n";

    // ── push_back ─────────────────────────────────────
    std::cout << "\n-- push_back --\n";
    dsa::String d;
    for (char ch : {'D','S','A'}) d.push_back(ch);
    std::cout << "push_back D,S,A = " << d << "\n";

    // ── find ──────────────────────────────────────────
    std::cout << "\n-- find --\n";
    dsa::String text{"the quick brown fox"};
    auto pos = text.find("brown");
    std::cout << "find(\"brown\") in \"" << text << "\" = index " << pos << "\n";
    pos = text.find("cat");
    std::cout << "find(\"cat\") = "
              << (pos == dsa::String::npos ? "npos" : std::to_string(pos)) << "\n";

    // ── substr ────────────────────────────────────────
    std::cout << "\n-- substr --\n";
    dsa::String sub = text.substr(4, 5);
    std::cout << "substr(4,5) = \"" << sub << "\"\n";

    // ── Comparison ────────────────────────────────────
    std::cout << "\n-- Comparison --\n";
    dsa::String x{"apple"};
    dsa::String y{"banana"};
    std::cout << "\"apple\" == \"apple\" : " << (x == x ? "true" : "false") << "\n";
    std::cout << "\"apple\" != \"banana\": " << (x != y ? "true" : "false") << "\n";
    std::cout << "\"apple\" <  \"banana\": " << (x <  y ? "true" : "false") << "\n";

    // ── Copy vs Move ──────────────────────────────────
    std::cout << "\n-- Copy vs Move --\n";
    dsa::String src{"original"};
    dsa::String copied = src;
    dsa::String moved  = std::move(src);
    std::cout << "copied = " << copied << "\n";
    std::cout << "moved  = " << moved  << "\n";
    std::cout << "src after move = \"" << src << "\" size=" << src.size() << "\n";

    std::cout << "\n[DSA-CPP] Demo complete.\n";
    return 0;
}