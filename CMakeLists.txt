cmake_minimum_required (VERSION 2.8)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Werror -Wextra -Wsign-compare -Wsign-conversion -Wcast-qual -Wconversion -Wnon-virtual-dtor -pedantic" )

# For Clang, use libc++ and color-diagnostics
if ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fcolor-diagnostics" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics" )
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY .. )

include_directories( include test_src )

add_subdirectory   (         test_src )
