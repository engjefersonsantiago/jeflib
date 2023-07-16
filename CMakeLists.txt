cmake_minimum_required(VERSION 3.16)

project(jeflib)

ENABLE_TESTING()

add_subdirectory(common)
add_subdirectory(bitset)
add_subdirectory(allocator)
add_subdirectory(iterator)
add_subdirectory(vector)
add_subdirectory(list)

include(CodeCoverage.cmake)

APPEND_COVERAGE_COMPILER_FLAGS()
SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME coverage                 
        EXECUTABLE ctest -j 2 # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES
        bitset_test
        allocator_test
)
