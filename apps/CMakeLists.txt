link_directories (${CMAKE_SOURCE_DIR}/hypar)

file(GLOB_RECURSE APP_SOURCES "*.cpp")

find_package(Boost CONFIG)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(APPSLIST "")
foreach(appfile ${APP_SOURCES})
    get_filename_component(appname ${appfile} NAME_WE)
    add_executable(${appname} ${appfile})
    target_link_libraries(${appname} ${LIB_NAME})
    list(APPEND APPSLIST ${appname})
endforeach()

install (FILES
    ${APPSLIST}
DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ
GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
