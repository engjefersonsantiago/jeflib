cmake_minimum_required(VERSION 3.5)

project(biset)


# Set a default C++ compile flag
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2 -std=c++20 -Wpedantic -Werror -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined -g -ftest-coverage -fno-inline -O0 -fno-elide-constructors")

############################################################
# Create an executable
############################################################

## Test target
include(CTest)
enable_testing()

find_package(GTest REQUIRED)

# Add a testing executable

add_executable(list_test list_tags unittest/list_test.cpp)

target_include_directories(list_test
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(list_test
    PRIVATE
    GTest::GTest
    GTest::Main
)

add_test(test_all list_test)

#include(CodeCoverage.cmake)
#APPEND_COVERAGE_COMPILER_FLAGS()
#SETUP_TARGET_FOR_COVERAGE_LCOV(
#        NAME biset_coverage                 
#        EXECUTABLE ctest -j 2 # Executable in PROJECT_BINARY_DIR
#        DEPENDENCIES
#        common_test
#            )
#

## Custom CTAGS
# Add a top-level "tags" target which includes all files in both
# the build and source versions of src/*.
set_source_files_properties(list_tags PROPERTIES GENERATED true)
add_custom_target(list_tags
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q
        ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
        #COMMAND ln -sf ${CMAKE_CURRENT_BINARY_DIR}/tags ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# ...but only make it a dependency of the project if the ctags program
# is available, else it will fail to build on Windows.
find_program(CTAGS_PATH ctags)
if(CTAGS_PATH)
    message(STATUS "Found ctags: ${CTAGS_PATH}")
    add_dependencies(list_test list_tags)
endif(CTAGS_PATH)

# Build Docs
# Require dot, treat the other components as optional
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)

doxygen_add_docs(
    list_doxygen
    ${PROJECT_SOURCE_DIR}
    COMMENT "Generate doc pages"
)
