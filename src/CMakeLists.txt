include_directories(${CMAKE_CURRENT_SOURCE_DIR})
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/hypar/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/hypar/config.h)

add_subdirectory (hypar)
add_subdirectory (test)
add_subdirectory (apps)
