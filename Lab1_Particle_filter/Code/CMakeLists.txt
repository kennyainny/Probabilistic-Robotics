cmake_minimum_required(VERSION 2.8.9)
project (Lab1 C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_INSTALL_RPATH "$ORIGIN")
SET(CMAKE_EXE_LINKER_FLAGS
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")
set( CMAKE_SKIP_BUILD_RPATH true )
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/main.hpp
	${PROJECT_SOURCE_DIR}/src/def_type.h
	${PROJECT_SOURCE_DIR}/src/map.c
	${PROJECT_SOURCE_DIR}/src/map.h
	${PROJECT_SOURCE_DIR}/src/sensor.c
	${PROJECT_SOURCE_DIR}/src/sensor.h
	${PROJECT_SOURCE_DIR}/src/distribution.c
	${PROJECT_SOURCE_DIR}/src/distribution.h
	${PROJECT_SOURCE_DIR}/src/visualize.cpp
	${PROJECT_SOURCE_DIR}/src/visualize.hpp
	${PROJECT_SOURCE_DIR}/src/motion.cpp
	${PROJECT_SOURCE_DIR}/src/motion.hpp
	${PROJECT_SOURCE_DIR}/src/particle.cpp
	${PROJECT_SOURCE_DIR}/src/particle.hpp)

add_executable(lab1 ${SOURCE_FILES})
target_link_libraries(lab1 ${OpenCV_LIBS})
