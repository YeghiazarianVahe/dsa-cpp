cmake_minimum_required(VERSION 3.25)
project(dsa_cpp VERSION 0.1.0 LANGUAGES CXX)

# ── Global C++20 standard ────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # Disable GNU extensions for portability

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX /O2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
endif()

# ── Export compile_commands.json (for clangd / VS Code IntelliSense) ─────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Subprojects (uncomment as we implement each topic) ───────────────────────
add_subdirectory(setup)

# Phase 1
# add_subdirectory(phase1/complexity_analysis)
# add_subdirectory(phase1/arrays)
# add_subdirectory(phase1/sorting)
# add_subdirectory(phase1/binary_search)

# Phase 2 — uncomment as we reach each topic
# add_subdirectory(phase2/linked_list)
# ...

# Phase 3
# add_subdirectory(phase3/bst)
# ...

# Phase 4
# add_subdirectory(phase4/dynamic_programming)
# ...
