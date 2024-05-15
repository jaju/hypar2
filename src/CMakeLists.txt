#
# Copyright 2011 Ravindra Jaju (jaju [AT] msync org)
#
file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
file(GLOB_RECURSE SRC_FILES "*.cpp")

option(DEBUG "Build the project with debug calls" ON)
check_include_files(list HAVE_CXX_LIST)
check_function_exists(exit HAVE_EXIT)
find_package(Iconv REQUIRED)
add_library(${LIB_NAME} SHARED ${SRC_FILES})

if(Iconv_FOUND)
  include_directories(${Iconv_INCLUDE_DIRS})
  target_link_libraries(${LIB_NAME} ${Iconv_LIBRARIES})
endif()


install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES
    ${HEADER_FILES}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hypar
	PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
