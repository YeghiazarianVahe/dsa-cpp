add_executable(test_string
    tests/test_string.cpp
    String.cpp
)
target_include_directories(test_string PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(test_string PRIVATE cxx_std_20)

add_executable(string_demo
    examples/string_demo.cpp
    String.cpp
)
target_include_directories(string_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(string_demo PRIVATE cxx_std_20)