cmake_minimum_required(VERSION 2.8.9)
project (Lab4 C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

set(SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/main.hpp
	${PROJECT_SOURCE_DIR}/src/def_type.h
	${PROJECT_SOURCE_DIR}/src/visualize.cpp
	${PROJECT_SOURCE_DIR}/src/visualize.hpp
	${PROJECT_SOURCE_DIR}/src/log.cpp
	${PROJECT_SOURCE_DIR}/src/log.hpp
	${PROJECT_SOURCE_DIR}/src/GP_RBF.cpp
	${PROJECT_SOURCE_DIR}/src/GP_RBF.hpp
	${PROJECT_SOURCE_DIR}/src/GP_RQ.cpp
	${PROJECT_SOURCE_DIR}/src/GP_RQ.hpp
	${PROJECT_SOURCE_DIR}/src/baysian.cpp
	${PROJECT_SOURCE_DIR}/src/baysian.hpp
	${PROJECT_SOURCE_DIR}/src/regret.cpp
	${PROJECT_SOURCE_DIR}/src/regret.hpp)

add_executable(lab4 ${SOURCE_FILES})
target_link_libraries(lab4 ${OpenCV_LIBS} ${PCL_LIBRARIES})
