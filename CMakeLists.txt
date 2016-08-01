cmake_minimum_required (VERSION 2.8.8)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Werror -Wextra -Wsign-compare -Wcast-qual -Wconversion -Wnon-virtual-dtor -pedantic" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY .. )

include_directories( include test_src )

add_subdirectory   (         test_src )
