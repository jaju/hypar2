# http://code.google.com/p/createtetzone/source/browse/cmake/modules/FindGMock.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

include_directories(${GMOCK_INC})
include_directories(${GTEST_INCLUDE_DIRS})

link_libraries (hypar)

add_executable (charentitytest charentitytest.cpp)
add_executable (charstreamtest charstreamtest.cpp)
add_executable (iconvtest iconvtest.cpp)
add_executable (mmaptest mmaptest.cpp)
add_executable (saxtimertest saxtimertest.cpp)
add_executable (tokenizertest tokenizertest.cpp)

add_test(charentitytest charentitytest charentitytest.cpp)
