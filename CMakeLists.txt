cmake_minimum_required(VERSION 2.8)
project (hypar CXX C)

include (CheckIncludeFiles)
include (CheckFunctionExists)

set(COVERAGE_FLAGS "-O0 -pg -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_FLAGS}")

enable_testing()
add_subdirectory (src)

# add lcov target
include(${CMAKE_SOURCE_DIR}/cmake/lcov.cmake)