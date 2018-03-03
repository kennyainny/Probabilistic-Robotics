#include "main.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

	printf("Hello Lab2\n");
	
	/* Read Log Data */
	log_type log1, log2;
	std::string Log1_name = "../data/oakland_part3_am_rf.node_features";
	std::string Log2_name = "../data/oakland_part3_an_rf.node_features";
	read_log(Log1_name.c_str(), &log1);
	read_log(Log2_name.c_str(), &log2);

	// printf("%lu\n", log1.count);
	// for(int i = 0; i < log1.count; i++){
	// 	std::cout << log1.point[i].x << ' ';
	// 	std::cout << log1.point[i].y << ' ';
	// 	std::cout << log1.point[i].z << ' ';
	// 	std::cout << log1.node_id[i] << ' ';
	// 	std::cout << log1.node_label[i] << ' ';
	// 	for(int j = 0; j < 9; j++){
	// 		std::cout << log1.feature[i].f[j] << ' ';
	// 	}
	// 	std::cout << log1.feature[i].bias;
	// 	std::cout << '\n';
	// }

	/* Gradient Descent on Squared Loss */



	/* Baysian Linear Regression */



	/* NN ? Logistic Regression ? SVM */



	/* Visualization using PCL */
	data_visualization(log1);



	// waitkey(0); //uncomment this to not closing the figure
	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}

// cmake_minimum_required(VERSION 2.8.9)
// project (Lab1 C CXX)
// set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

// set(CMAKE_INSTALL_RPATH "$ORIGIN")
// SET(CMAKE_EXE_LINKER_FLAGS
//           "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")
// set( CMAKE_SKIP_BUILD_RPATH true )
// set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

// set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
// set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
// #set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)

// find_package(OpenCV REQUIRED)
// include_directories(${OpenCV_INCLUDE_DIRS})

// find_package(PCL REQUIRED)
// include_directories(${PCL_INCLUDE_DIRS})
// link_directories(${PCL_LIBRARY_DIRS})
// add_definitions(${PCL_DEFINITIONS})

// set(SOURCE_FILES 
// 	${PROJECT_SOURCE_DIR}/src/main.cpp
// 	${PROJECT_SOURCE_DIR}/src/main.hpp
// 	${PROJECT_SOURCE_DIR}/src/def_type.h
// 	${PROJECT_SOURCE_DIR}/src/visualize.cpp
// 	${PROJECT_SOURCE_DIR}/src/visualize.hpp
// 	${PROJECT_SOURCE_DIR}/src/log.cpp
// 	${PROJECT_SOURCE_DIR}/src/log.hpp)

// add_executable(lab2 ${SOURCE_FILES})
// target_link_libraries(lab2 ${OpenCV_LIBS} ${PCL_LIBRARIES})