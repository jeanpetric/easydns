cmake_minimum_required (VERSION 2.8)
project (EasyDNS)

option (DEBUG "Print detailed warnings during compile" 0)

set (EasyDNS_VERSION_MAJOR 0)
set (EasyDNS_VERSION_MINOR 1)

if (DEBUG)
	add_definitions(-Wall)
	add_definitions(-Wextra)
	add_definitions(-pedantic)
endif (DEBUG)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory (src)
