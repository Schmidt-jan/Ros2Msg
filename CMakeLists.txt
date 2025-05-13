cmake_minimum_required(VERSION 3.10)
project(SerdeTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-Dateien
include_directories(include)

# Tests aktivieren
enable_testing()

set(CMAKE_BUILD_TYPE Debug)

# Alle Testdateien automatisch einlesen
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    tests/*_test.cpp
)

# Test-Executable
add_executable(run_tests ${TEST_SOURCES})

# GTest (vorausgesetzt, du nutzt Googletest über FetchContent oder Submodul)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(run_tests gtest gtest_main)

# Test registrieren
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
