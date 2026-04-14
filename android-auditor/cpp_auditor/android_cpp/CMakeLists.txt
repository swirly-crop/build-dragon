cmake_minimum_required(VERSION 3.10)
project(android_cpp_auditor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(auditor main.cpp)

find_library(log-lib log)
if(log-lib)
    target_link_libraries(auditor ${log-lib})
endif()
