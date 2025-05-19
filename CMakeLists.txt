cmake_minimum_required(VERSION 3.14)
project(SerdeTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Includes
include_directories(include)

# Enable tests
enable_testing()

# =======================
# GTest for C++ Unit Tests
# =======================
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    tests/*_test.cpp
)

# Fetch GTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Build test executable
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests gtest gtest_main)

# Register tests
add_test(NAME AllTests COMMAND run_tests)
add_test(NAME ValgrindTests
    COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:run_tests>
)
add_test(NAME Valgrind_ByteMultiArray
    COMMAND valgrind
        --leak-check=full
        --track-origins=yes
        --error-exitcode=1
        $<TARGET_FILE:run_tests>
        --gtest_filter=ByteMultiArrayTest.*
)

# =======================
# Pybind11 Python Binding
# =======================
# Add pybind11 via FetchContent (or use find_package if installed)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(pybind11)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(ros2msg bindings/bindings.cpp)
install(TARGETS ros2msg DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
