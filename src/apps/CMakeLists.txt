link_directories (${CMAKE_SOURCE_DIR}/hypar)
link_libraries (hypar-static)

add_executable (hyhtml2text  hyhtml2text.cpp)
add_executable (hyhtmldoc  hyhtmldoc.cpp)
add_executable (hyurlextract  hyurlextract.cpp)
add_executable (hyxmlclean  hyxmlclean.cpp)
add_executable (googleextract  googleextract.cpp)

install (FILES
hyhtml2text
hyhtmldoc
hyurlextract
hyxmlclean
googleextract
DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ
GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
