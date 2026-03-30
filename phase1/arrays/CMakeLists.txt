# phase1/arrays/CMakeLists.txt

add_executable(test_dynamic_array
    tests/test_dynamic_array.cpp
    DynamicArray.cpp
)
target_include_directories(test_dynamic_array PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(test_dynamic_array PRIVATE cxx_std_20)

add_executable(dynamic_array_demo
    examples/dynamic_array_demo.cpp
    DynamicArray.cpp
)
target_include_directories(dynamic_array_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(dynamic_array_demo PRIVATE cxx_std_20)