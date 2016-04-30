cmake_minimum_required(VERSION 2.8)

# Name the application
SET( PROJ_NAME      "rummyGin" )

project( $(PROJ_NAME) )

# Compiler Settings
add_definitions( -std=c++14 -O2 )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Enable Tests for CMake
include(CTest)

add_subdirectory( src )
add_subdirectory( test )
