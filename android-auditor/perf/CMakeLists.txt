cmake_minimum_required(VERSION 3.10)
project(perf_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(perf_benchmark perf_benchmark.cpp)

# On aarch64 include the neon assembly implementation
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    message(STATUS "Building with aarch64 neon memcpy")
    add_library(neon_memcpy STATIC ../assembly/hello_aarch64.s)
    target_sources(perf_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/../assembly/hello_aarch64.s)
    # assembly will be compiled and linked automatically by CMake
endif()
