set (hypar_LIB_SRCS
charentitystream.cpp
debug.cpp
domnode.cpp
domutils.cpp
entitystack.cpp
hash.cpp
html.cpp
htmltable.cpp
iconv.cpp
mbtowide.cpp
mmap.cpp
tag.cpp
tagentry.cpp
tagtable.cpp
textutils.cpp
tree.cpp
urlcollector.cpp
url.cpp
xml.cpp
)

option (DEBUG "Build the project with debug printing on" ON)
check_include_files (list HAVE_CXX_LIST)
check_function_exists (mmap HAVE_MMAP)

add_library (hypar SHARED ${hypar_LIB_SRCS})
add_library (hypar-static STATIC ${hypar_LIB_SRCS})

set_target_properties(hypar PROPERTIES OUTPUT_NAME hypar)
set_target_properties(hypar-static PROPERTIES OUTPUT_NAME hypar-static)

install ( FILES
charentitystream.hpp
debug.hpp
domnode.hpp
domutils.hpp
entitystack.hpp
hash.hpp
hashmap.hpp
hashset.hpp
html.hpp
iconv.hpp
mbtowide.hpp
mmap.hpp
tagentry.hpp
tag.hpp
tagtable.hpp
textutils.hpp
tokenizer.hpp
url.hpp
xml.hpp
config.h
localdefs.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hypar
	PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install ( FILES
	libhypar.so
	libhypar-static.a
	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
