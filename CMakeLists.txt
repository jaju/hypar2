#
# Copyright 2011 Ravindra Jaju (jaju [AT] msync org)
#
cmake_minimum_required(VERSION 3.29)
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
###########################################################
###########################################################
project(hypar C CXX)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
enable_testing()
###########################################################
###########################################################
include(CheckIncludeFiles)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(FindIconv)
find_package(Iconv REQUIRED)
include_directories(${SRC_DIR})
configure_file(${SRC_DIR}/config.h.in
    ${SRC_DIR}/config.h)

set(LIB_NAME hypar)
option(BUILD_WITH_COVERAGE "Build the project with code coverage on" OFF)
if(BUILD_WITH_COVERAGE)
    SET(COMPILER_COVERAGE_FLAGS "-pg -O0 -Wall -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILER_COVERAGE_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_COVERAGE_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
    include(${CMAKE_SOURCE_DIR}/cmake/lcov.cmake)
endif(BUILD_WITH_COVERAGE)

# Set C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Turn on to ensure -std=c++11 not -std=gnu++11

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(apps)
