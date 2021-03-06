cmake_minimum_required(VERSION 2.8.9)
project (Project C CXX)
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

find_package(PythonLibs 2.7)
include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/main.hpp
	${PROJECT_SOURCE_DIR}/src/def_type.hpp
	${PROJECT_SOURCE_DIR}/src/visualize.cpp
	${PROJECT_SOURCE_DIR}/src/visualize.hpp
	${PROJECT_SOURCE_DIR}/src/trajectory.cpp
	${PROJECT_SOURCE_DIR}/src/trajectory.hpp
	${PROJECT_SOURCE_DIR}/src/distribution.cpp
	${PROJECT_SOURCE_DIR}/src/distribution.hpp
	${PROJECT_SOURCE_DIR}/src/sensor.cpp
	${PROJECT_SOURCE_DIR}/src/sensor.hpp
	${PROJECT_SOURCE_DIR}/src/motion.cpp
	${PROJECT_SOURCE_DIR}/src/motion.hpp
	${PROJECT_SOURCE_DIR}/src/particle.cpp
	${PROJECT_SOURCE_DIR}/src/particle.hpp
	${PROJECT_SOURCE_DIR}/src/matplotlibcpp.h)

add_executable(project ${SOURCE_FILES})
target_link_libraries(project ${OpenCV_LIBS} ${PYTHON_LIBRARIES})
