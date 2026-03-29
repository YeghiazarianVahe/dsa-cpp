# phase1/complexity_analysis/CMakeLists.txt

# ── Test executable ───────────────────────────────────────────────────────────
add_executable(test_complexity
    tests/test_complexity.cpp
    Benchmark.cpp
)
target_include_directories(test_complexity PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(test_complexity PRIVATE cxx_std_20)

# ── Demo executable ───────────────────────────────────────────────────────────
add_executable(complexity_demo
    examples/complexity_demo.cpp
    Benchmark.cpp
)
target_include_directories(complexity_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(complexity_demo PRIVATE cxx_std_20)