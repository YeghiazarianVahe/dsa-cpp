/**
 * @file  examples/dynamic_array_demo.cpp
 *
 * Compile:
 *   g++ -std=c++20 -Wall -Wextra -O2 \
 *       examples/dynamic_array_demo.cpp ../DynamicArray.cpp -o dynamic_array_demo
 */

#include "../DynamicArray.h"
#include <iostream>
#include <format>

static void print(const std::string& label, const dsa::DynamicArray<int>& a) {
    std::cout << std::format("{:20s} | size={:2d} cap={:2d} | [",
                             label, a.size(), a.capacity());
    for (std::size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i];
        if (i + 1 < a.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    std::cout << "=== DynamicArray Demo ===\n\n";

    // ── 1. Growth behaviour ───────────────────────────
    std::cout << "-- Growth (watch capacity double) --\n";
    dsa::DynamicArray<int> a;
    for (int i = 1; i <= 9; ++i) {
        a.push_back(i * 10);
        print(std::format("after push_back({})", i * 10), a);
    }

    // ── 2. insert / erase ─────────────────────────────
    std::cout << "\n-- Insert & Erase --\n";
    print("before insert", a);
    a.insert(2, 999);
    print("insert(2, 999)", a);
    a.erase(2);
    print("erase(2)", a);

    // ── 3. reserve / resize ───────────────────────────
    std::cout << "\n-- Reserve & Resize --\n";
    dsa::DynamicArray<int> b;
    b.reserve(16);
    print("after reserve(16)", b);
    b.resize(5, 42);
    print("after resize(5,42)", b);
    b.resize(3);
    print("after resize(3)", b);

    // ── 4. Copy vs Move ───────────────────────────────
    std::cout << "\n-- Copy vs Move --\n";
    dsa::DynamicArray<int> src;
    src.push_back(1); src.push_back(2); src.push_back(3);

    dsa::DynamicArray<int> copied = src;
    dsa::DynamicArray<int> moved  = std::move(src);

    print("copied", copied);
    print("moved",  moved);
    print("src after move", src);   // must be empty

    // ── 5. Range-for via iterators ────────────────────
    std::cout << "\n-- Range-for --\n";
    dsa::DynamicArray<int> c;
    for (int i = 1; i <= 5; ++i) c.push_back(i);
    std::cout << "elements: ";
    for (int v : c) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n[DSA-CPP] Demo complete.\n";
    return 0;
}