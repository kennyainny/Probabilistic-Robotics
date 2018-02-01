cmake_minimum_required(VERSION 2.8.9)
project (Lab1)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)

set(SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/src/HelloWorld.c)

add_executable(lab1 ${SOURCE_FILES})
