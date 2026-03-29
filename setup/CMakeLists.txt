# setup/CMakeLists.txt
add_executable(hello_dsa main.cpp)

# Confirm C++20 features compile (std::format requires it)
target_compile_features(hello_dsa PRIVATE cxx_std_20)
