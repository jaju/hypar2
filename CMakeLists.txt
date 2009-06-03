cmake_minimum_required(VERSION 2.6)
project (hypar CXX C)
include (CheckIncludeFiles)
include (CheckFunctionExists)
include_directories (${CMAKE_SOURCE_DIR}/src)
configure_file (${CMAKE_SOURCE_DIR}/src/hypar/config.h.in
${CMAKE_SOURCE_DIR}/src/hypar/config.h)
add_subdirectory (src)
